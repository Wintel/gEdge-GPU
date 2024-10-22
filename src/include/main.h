/*******************************************************************************
        ClusterGL - main.h
*******************************************************************************/

// #define ENABLE_DL_INTERCEPT

#ifdef __APPLE__
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-parameter"
// #pragma clang diagnostic pop
#endif

/*******************************************************************************
        Internal headers
*******************************************************************************/
#include "config.h"
#include "instruction.h"
#include "libs.h"
#include "lru_cache.h"
#include "module.h"
#include "mongoose.h"
#include "utils.h"
#include <SDL/SDL.h>
#include <cstdio>
#include <nvml.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#pragma comment(lib, "nvml");

/*******************************************************************************
        Statistics collector
*******************************************************************************/
class Stats {
  static void output();

public:
  static void count(string key, int count);
  static void increment(string key, int count = 1);
  static void update();
};

/*******************************************************************************
        Main application object
*******************************************************************************/
class App {

  void init(bool shared, const char *id);

  void stats_begin();
  void stats_end();

public:
  // called when we're invoked from the command line
  int run(int argc, char **argv);
  int index;
  pthread_t tid;

  vector<Module *> mModules;
  vector<int> portNum;

  // called when we're invoked from LD_PRELOAD
  // Will return false if we're not configured to run off this source
  bool run_shared(string src);

  int GPU_util();

  long double getNetworkBandwidth();

  bool tick();

  void debug();
};

extern bool bIsIntercept;

// Global config instance
extern Config *gConfig;

// App *theApp = NULL;

// mod_text
void LOG_INSTRUCTION(Instruction *instr);

// automatically generated, consts.cpp
const char *getGLParamName(unsigned int param);
