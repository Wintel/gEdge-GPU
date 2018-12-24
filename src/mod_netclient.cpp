/*******************************************************************************
	Network client module
*******************************************************************************/

#include "main.h"


const int SEND_BUFFER_SIZE = 268435456;//1294276

uint32_t iSendBufPos = 0;
uint32_t bytesLeft = SEND_BUFFER_SIZE;

//big buffer to store instructions before sending
byte mSendBuf[SEND_BUFFER_SIZE];
extern  App *theApp;
int totalSent;

/*******************************************************************************
	Network client module connect / setup
*******************************************************************************/
NetClientModule::NetClientModule()
{    
	//Make each socket
	for(int i=0;i<gConfig->numOutputs;i++){	
		struct addrinfo hints, *res;
	
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		
		string addr = gConfig->outputAddresses[i];
		int port =theApp->portNum[i];
		
		LOG("port number: %d,ip address: %s\n",port,gConfig->outputAddresses[i].c_str());
		getaddrinfo(addr.c_str(), toString(port).c_str(), &hints, &res);
		
		int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		
		if(s == 0){
			LOG("Couldn't make socket!\n");
			return;
		}
				
		//set TCP options 
		int one = 1;

		setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
		setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
				
		struct sockaddr_in mAddr; 
        
	//	int c = connect(s, res->ai_addr, res->ai_addrlen);
        while(connect(s, res->ai_addr, res->ai_addrlen)<0);
	//    LOG("connection result %d\n",c);
	//    if(c < 0) {
	//		LOG("Failed to connect with server '%s:%d' - error %s\n", 
	//				addr.c_str(), port, strerror( errno ));
			//exit(1);
	//		continue;
	//	}
	  
	
		mSockets.push_back(s);
		mConnect.push_back(true);
		consumption.push_back(0.0);

		LOG("Connected to remote pipeline on %s:%d\n", addr.c_str(), port);	

	}

}



/*******************************************************************************
	Send each instruction
*******************************************************************************/
bool NetClientModule::process(vector<Instruction *> *list)
{
	
	//First send the total number
	uint32_t num = list->size();
		
	if(!internalWrite(&num, sizeof(uint32_t))) {
		LOG("Connection problem!\n");
		return false;
	}

	//Now send the instructions
	int counter = 0;
	for(int n=0;n<(int)list->size();n++){
		
		Instruction *i = (*list)[n];
		
		//Check for NULL buffers and reset length if necessary
		for(int n=0;n<3;n++) {
			int l = i->buffers[n].len;

			if(l > 0 && !i->buffers[n].buffer) {
				i->buffers[n].len = 0;
			}
		}			
		
		//now the struct itself	
		if(internalWrite(i, sizeof(Instruction)) != sizeof(Instruction)) {
			LOG("Connection problem (didn't send instruction)!\n");
			return false;
		}
		//Now see if we need to send any buffers
       
		for(int n=0;n<3;n++) {
			int l = i->buffers[n].len;
			//LOG("recv buffer %d\n",l);
			if(l > 0) {
				if(internalWrite(i->buffers[n].buffer, l) != l) {
					LOG("Connection problem (didn't write buffer %d)!\n", l);
					return false;
				}
				//And check if we're expecting a buffer back in response
				if(i->buffers[n].needReply) {
                    
					sendBuffer();
					if(i->id==256)
			        {
				     l = 5;
			        }
					int x=internalRead(i->buffers[n].buffer, l);
					if(x!= l) 
					{
						LOG("Connection problem: NetClient (didn't recv buffer %d got: %d, instruction: %d)!\n", l, x, i->id);
						return false;
					}
				}
			}
		}
		counter++;
		LOG("done\n");
	}
	

	sendBuffer(); //send anything that's outstanding
	Stats::count("mod_netclient write", totalSent);
	totalSent = 0;

	return true;
}



/*******************************************************************************
 Sync
*******************************************************************************/
bool NetClientModule::sync()
{
    LOG("NetClientModule::sync() requested, but not implemented\n");
	
	return true;
}


int NetClientModule::min_func()
{
	int index=0;
	double timevalue=0;
	for(int i=0;i<consumption.size();i++)
	{
		if(consumption[i]<timevalue)
		{
		   timevalue = consumption[i];
		   index = i;
		}
	}
	return index;
}

/*******************************************************************************
 Write to the network
*******************************************************************************/
int NetClientModule::internalWrite(void* buf, int nByte)
{   		
	if(bytesLeft - nByte > 0) {
		memcpy(mSendBuf + iSendBufPos, buf, nByte);
		iSendBufPos += nByte;
		bytesLeft -= nByte;
	}
	else {
		sendBuffer();
	}
	return nByte;
}

void NetClientModule::sendBuffer()
{
	static int count=0;

	if(iSendBufPos == 0){
		return;
	}
	
	byte *mCompressedBuf = NULL;
	
	if(gConfig->networkCompression){
		LOG("Compressed!!!\n");		
		mCompressedBuf = Compression::getBuf();
		iSendBufPos = Compression::compress(mSendBuf, iSendBufPos);
	}
	
	for(int i=0;i<(int)mSockets.size();i++){
		
		 if(!mConnect[i])
		    return;
		 uint32_t a = write(mSockets[i], &iSendBufPos, sizeof(iSendBufPos));
		 uint32_t b = 0;	
		 clock_t time0;
		 clock_t time1;
		 time0 = clock();
		 if(!gConfig->networkCompression){		
			b = write(mSockets[i], mSendBuf, iSendBufPos);			
		 }else{
			b = write(mSockets[i], mCompressedBuf, iSendBufPos);
		 }
	 	time1 = clock()-time0;
	    double time_taken = ((double)time1)/CLOCKS_PER_SEC;
	/*	if(time_taken>0.0001)
		{
		    close(mSockets[i]);
			mConnect[i] = false;
			std::cout<<(float)time_taken<<std::endl;
		}*/
		 if(a != sizeof(iSendBufPos) && b != iSendBufPos){
			LOG("Failure to send: %d/%d\n", i, iSendBufPos);
		 }	
			
		 totalSent += a;
		 totalSent += b;
	}
	iSendBufPos = 0;
	bytesLeft = SEND_BUFFER_SIZE;
	if(count!=-1)
	   count++;
}



/*******************************************************************************
 Read from the network
*******************************************************************************/
int NetClientModule::internalRead(void *buf, size_t count)
{
	LOG("About to read %d\n", count);
	uint32_t n = 0;
	//Read from each renderer
	for(int i=0;i<(int)mSockets.size();i++){
		n = 0;
		byte *d = (byte *)buf;
		while(n < count){
		    n += read(mSockets[i], d + n, count);		  
		    LOG("%d\n", n);
		}		
	}
	return n;
}
