/*******************************************************************************
        libs.h
*******************************************************************************/

/*********************************************
        STL
**********************************************/
#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using std::list;
using std::map;
using std::stack;
using std::string;
using std::vector;

/*********************************************
        C stdlib
**********************************************/
#include <arpa/inet.h>
#include <dlfcn.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <confuse.h>

#include <errno.h>

#include <zconf.h>
#include <zlib.h>

/*********************************************
        OpenGL
**********************************************/

// Sometimes we don't want the GL headers interfering
#ifndef NO_OPENGL_HEADERS
#ifdef __APPLE__
#include <SDL/SDL_opengl.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif
#endif

/*********************************************
        SDL*
**********************************************/
#include <SDL/SDL.h>
#include <SDL/SDL_mutex.h>
#include <SDL/SDL_net.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_thread.h>

typedef unsigned char byte;

#define MAX(a, b) ((a < b) ? (b) : (a))
#define MIN(a, b) ((((a) - (b)) & 0x80000000) >> 31) ? (a) : (b)
