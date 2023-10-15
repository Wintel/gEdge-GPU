
/*******************************************************************************
 libcglinput
*******************************************************************************/

#include <dlfcn.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

#define LOG printf

/*******************************************************************************
 Main functions
*******************************************************************************/
extern bool run(string methods);
extern bool begin_web();
extern bool begin_spacenav();

/*******************************************************************************
 Configuration
*******************************************************************************/
static const int MAX_AXIS = 512;

class Config {
public:
  Config(string filename);
  int axis_actions[MAX_AXIS][2];
  string device;
  int num_axis;
  int thresh[MAX_AXIS];
};

extern Config *mConfig;

/*******************************************************************************
 Injection interface
*******************************************************************************/

class IInject {
public:
  virtual void keydown(int keycode) = 0;
  virtual void keyup(int keycode) = 0;
};

class SDLInject : public IInject {
public:
  void keydown(int keycode);
  void keyup(int keycode);
};

class XInject : public IInject {
public:
  void keydown(int keycode);
  void keyup(int keycode);
};

extern IInject *mInject;
