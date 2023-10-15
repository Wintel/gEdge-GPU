
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WINDOWS
#include <Winsock2.h>
#include <time.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

// libconfuse
#include <confuse.h>

/*********************************************
                                        OpenGL
**********************************************/
#ifdef ENABLE_CGL_COMPAT
#define GL_GLEXT_PROTOTYPES
#else
#include <GL/glew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

/*********************************************
                                SDL*
**********************************************/
#ifdef _WINDOWS
#include <SDL.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_mutex.h>
#include <SDL/SDL_net.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_ttf.h>
#endif

#define ILUT_USE_OPENGL

// DevIL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
