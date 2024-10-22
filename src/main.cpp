/*******************************************************************************
	ClusterGL - main.cpp
*******************************************************************************/
#include "main.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <cstdlib>
/*******************************************************************************
	Globals
*******************************************************************************/
static bool bHasInit = false;
static long double networkusage = 0.0;
bool bIsIntercept = false;
Config *gConfig = NULL;
App *theApp = NULL;
//NetSrvModule* netserver=NULL;
 
/*******************************************************************************
	System monitor
*******************************************************************************/
long double App::getNetworkBandwidth()
{
	long double recv1 = 0.0,recv2 = 0.0;
    long double send1 = 0.0,send2 = 0.0;
    char name[32],buf[256];
    FILE *fp;

    fp = fopen("/proc/net/dev", "r");
    fgets(buf,255,fp);
    fgets(buf,255,fp);
    fscanf(fp,"%s %Lf %*LF %*LF %*LF %*LF %*LF %*LF %*LF %Lf",&name[0],&recv1,&send1);
    fgets(buf,255,fp);
    while(strcmp(name,"enp3s0:")!= 0){
        fscanf(fp,"%s %Lf %*LF %*LF %*LF %*LF %*LF %*LF %*LF %Lf",&name[0],&recv1,&send1);
        fgets(buf,255,fp);
    }
    fclose(fp);
    sleep(1);


    fp = fopen("/proc/net/dev", "r");
    fgets(buf,255,fp);
    fgets(buf,255,fp);
    fscanf(fp,"%s %Lf %*LF %*LF %*LF %*LF %*LF %*LF %*LF %Lf",&name[0],&recv2,&send2);
    fgets(buf,255,fp);
    while(strcmp(name,"enp3s0:")!= 0){
        fscanf(fp,"%s %Lf %*LF %*LF %*LF %*LF %*LF %*LF %*LF %Lf",&name[0],&recv2,&send2);
        fgets(buf,255,fp);
    }
    fclose(fp);
    long double usage = (recv2-recv1) / 1000.0 +(send2-send1)/1000.0;
	return usage;
}

int App:: GPU_util()
{
	nvmlReturn_t result;
    unsigned int device_count,utilization;

    result = nvmlInit();
    if (result != NVML_SUCCESS)
        return 1;
    
    result = nvmlDeviceGetCount(&device_count);
    if (result != NVML_SUCCESS)
        return 2;

    for (int i = 0; i < device_count; ++i) {
        nvmlDevice_t device;
        result = nvmlDeviceGetHandleByIndex(0, &device);
        if (result != NVML_SUCCESS)
            return 3;

        char device_name[NVML_DEVICE_NAME_BUFFER_SIZE];
        result = nvmlDeviceGetName(device, device_name, NVML_DEVICE_NAME_BUFFER_SIZE);
        if (result != NVML_SUCCESS)
            return 4;

        //std::printf("DenvmlDeviceGetNamevice %d: %s\n", i, device_name);

        nvmlUtilization_st device_utilization;
        result = nvmlDeviceGetUtilizationRates(device, &device_utilization);

        if (result != NVML_SUCCESS)
            return 5;

        //std::printf("GPU Util: %u, Mem Util: %u\n", device_utilization.gpu, device_utilization.memory);
		utilization = device_utilization.gpu;
        std::printf("GPU Util: %u\n",device_utilization.gpu);
    }
    nvmlShutdown();
    return utilization;
}

 void * pthread_usage(void *threadid)
 {
    while (1)
	{
       //theApp->GPU_util = GPU_util();
	   networkusage = theApp->getNetworkBandwidth();
	}
 }

/*******************************************************************************
	System Rendering
*******************************************************************************/

int App::run(int argc, char **argv)
{
	 int sockfd, portno,util;
    char buffer[256];
   struct sockaddr_in addr, clientaddr;
    int n;
	if (argc != 2) {
		fprintf(stderr,"usage: %s <window id>\n", argv[0]);
		exit(0);
	}
	
	if(bHasInit) {
		return 1;
	}
	
	init(false, argv[1]);
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
	{
         LOG("ERROR opening socket\n");
		 exit(1);
	}
    memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(gConfig->serverPort);
	//set TCP options
	portno = gConfig->serverPort;
	int one = 1;
	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

	//Bind the server socket
	if (bind(sockfd, (struct sockaddr *) &addr,
			sizeof(addr)) < 0) {
		LOG("Failed to bind the server socket NetSrvModule\n");
		exit(1);
	}
	//Listen
	if (listen(sockfd, 5) < 0) {
		LOG("Failed to listen on server socket\n");
		exit(1);
	}
    unsigned int clientlen = sizeof(clientaddr);
	int client = 0, pid;
	while(1)
	{
	  string port,utilRate,networkRate;
	 if ((client = accept(sockfd, (struct sockaddr *) &clientaddr, &clientlen)) < 0) {
		LOG("Failed to accept client connection\n");
		exit(1);
	  }
	  util = GPU_util();
      portno+=1; 
	  port=toString(portno);
	  utilRate=toString(util);
	  networkRate=toString((int)networkusage);
	  LOG("network usage %d\n",(int)networkusage);
	  char* info = (char*)(port+","+utilRate+","+networkRate).c_str();
      pid = fork();
	  if (pid < 0) {
              LOG("ERROR in new process creation\n");
			  exit(1);
      }
	  if(pid==0)
	  {
	    n = write(client, info, strlen(info));
		LOG("write return value: %s,%d\n",info,strlen(info));
        if (n < 0)
		{
          LOG("ERROR writing to socket");
		  exit(1);
		}
	    mModules.push_back(new NetSrvModule(portno));
        mModules.push_back(new DeltaDecodeModule()); 	
        mModules.push_back(new ExecModule());
	    while( tick() ){ 
	    }
		//close(sockfd);
		close(client);
	  }
	  else
	  {
		//close(sockfd);
		close(client); 
	  }
	}
	//Set up the module chain	
	//mModules.push_back(netserver);
	
	
	return 0;
}


/*******************************************************************************
	Entry if invoked as capture (shared library)
*******************************************************************************/
bool App::run_shared(string src)
{
	//This is needed to ensure that multiple calls to SDL_Init() don't cause
	//us to do the wrong thing.
	if(bHasInit) {
		return false;
	}

	char buffer[1024] = {0};
	int n=0;
	char* phase;
//	memset(&serv_addr, '0', sizeof(serv_addr));
	//Load the config file
	init(true, "capture");
	
	//Make sure we're launching from the correct source. This means that mixing
	//SDL and Xorg works properly when we're fork()'ing. ie: OpenArena
	if(src != gConfig->interceptMode){
		LOG("Ignored spurious launch (possibly fork()?): %s vs %s\n", 
			src.c_str(), gConfig->interceptMode.c_str());
		LOG("If this is intended, change 'interceptMode' to '%s' in the config file\n", src.c_str());
		return false;
	}

	if(gConfig->capturePidFile != ""){
		FILE *f = fopen(gConfig->capturePidFile.c_str(), "w");
		char pid[64];
		sprintf(pid, "%d", getpid());
		fwrite(pid, strlen(pid), 1, f);
		fclose(f);
	}
    
	for(int i=0;i<gConfig->numOutputs;i++){
		
		struct addrinfo hints, *res;
	
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		
		string addr = gConfig->outputAddresses[i];
		int port = gConfig->outputPorts[i];
		
		getaddrinfo(addr.c_str(), toString(port).c_str(), &hints, &res);
		
		int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		
		if(s == 0){
			LOG("Couldn't make socket!\n");
			exit(1) ;
		}	
		//set TCP options 
		int one = 1,portno;
		vector<int> util;

		setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
		setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
				
		struct sockaddr_in mAddr; 

		int c = connect(s, res->ai_addr, res->ai_addrlen);
        int count = 0;

		bzero(buffer,1024);
        
		//LOG("connection %d\n",c);
	    if(c < 0) {
			LOG("Failed to connect with server '%s:%d' - error %s\n", 
					addr.c_str(), port, strerror( errno ));
			//exit(1);
			portNum.push_back(0);
			continue;
		}
        
        n = read(s,buffer,1023);
        if (n < 0) 
		{
         LOG("ERROR reading from socket");
		 continue;
		}
		//LOG("buffer values:%s\n",buffer);
        phase = strtok(buffer,",");
		while(phase)
		{
		 // LOG("phase:%s\n",phase);
		  if(count==0)
		  {
           portno = atoi(phase);
		   portNum.push_back(portno);
		   //LOG("port number %d\n",portNum[i]);
		  }
		  else
		  {
			util.push_back(atoi(phase));
			//LOG("util rate %d\n",util[count-1]);
		  }
		  phase = strtok (NULL, ",");
		  count++;
		}
		if(util[0]<=90&&util[1]<=90)
		{
			index = i;
			//LOG("port number %d,%d\n",index,portNum.size());
			break;
		}		
	}
	
	//Write our pid out
	mModules.push_back(new AppModule(""));
	mModules.push_back(new DeltaEncodeModule());
	mModules.push_back(new NetClientModule());
	
	 		
	//Set up the module chain
	

	//Return control to the parent process.
	return true;
}

/*******************************************************************************
	Load the config file, init various internal variables
*******************************************************************************/
void App::init(bool shared, const char *id)
{
	printf("**********************************************\n");
	printf(" ClusterGL(%s - %s)\n", bIsIntercept ? "intercept" : "renderer", id);
	printf("**********************************************\n");	

	bIsIntercept = shared;
	
	char *configFile = (char *)"/etc/cgl.conf";
	
	if(getenv("CGL_CONFIG_FILE")){
		configFile = getenv("CGL_CONFIG_FILE");
	}

	gConfig = new Config(configFile, string(id ? id : "null"));
   // portNum.ClearAll();
	bHasInit = true;
	index = 0;
    int rc = pthread_create(&tid, NULL, pthread_usage, (void *)0);
	if (rc){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
}


/*******************************************************************************
	Main loop
*******************************************************************************/
bool App::tick()
{		
	vector<Instruction *> *thisFrame = new vector<Instruction *>();
	
	if(gConfig->enableStats){
		stats_begin();
	}

   // printf("mModules size %d\n",mModules.size());
	//Go through each module and process the frame
	for(int i=0;i<(int)mModules.size();i++) {
		Module *m = mModules[i];
		
		m->setListResult(thisFrame);
		
		if( !m->process(thisFrame) ) {
			LOG("Failed to process frame (in %d), bailing out\n", i);
			return false;
		}
	 //   LOG("mModules size %d\n",(int)mModules.size());

		//TODO: handle bytes and such
		thisFrame = m->resultAsList();
		
		if(!thisFrame){
			LOG("!thisFrame\n");
			return false;
		}
	}	
    
	//return appropriate frames
	for(int n=0;n<(int)thisFrame->size();n++){
		Instruction *iter = (*thisFrame)[n];
		for(int i=0;i<3;i++) {
			//If we need a reply, send it
			//But only do so if /we/ created the instruction
			if(iter->buffers[i].needReply && iter->buffers[i].needRemoteReply) {
				//LOG("need a reply %d\n", i);
				mModules[0]->reply(iter, i);
				//LOG_INSTRUCTION(iter);
				iter->buffers[i].needReply = false;
				if(iter->id==256)
				{
				 free(iter->buffers[i].buffer);
				}
				Stats::increment("Pipeline stalls due to replies");
			}
		}
	}

    

	if(gConfig->enableStats){
		//LOG("tick()\n");
		stats_end();
	}
	
	for(int i=0;i<(int)thisFrame->size();i++){
		bool mustdelete = false;
		Instruction *iter = (*thisFrame)[i];
		if(iter->id==CGL_REPEAT_INSTRUCTION)
			mustdelete = true;
		//LOG_INSTRUCTION(iter);
		iter->clear();

		// this has to be deleted on renderers only
		// because they dynamically copy instructions in DeltaDecodeModule
		if(!bIsIntercept || mustdelete)
			delete iter;
	}
	thisFrame->clear();

	delete thisFrame;
	
	//LOG("tick() done\n");

	return true;
}

/*******************************************************************************
	Begin stats run
*******************************************************************************/
void App::stats_begin(){
	
}

/*******************************************************************************
	End stats run
*******************************************************************************/
void App::stats_end(){
 	Stats::update();
}


/*******************************************************************************
	main()
*******************************************************************************/

int main( int argc, char **argv )
{
	theApp = new App();
    	
	int ret = theApp->run(argc, argv);
	delete theApp;
	return ret;
}

//The shared object entry is now in mod_app
