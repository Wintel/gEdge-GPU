/*
 * This code was created by Jeff Molofee '99
 * (ported to Linux/SDL by Ti Leggett '01)
 *
 * If you've found this code useful, please let me know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 *
 * or for port-specific comments, questions, bugreports etc.
 * email to leggett@eecs.tulane.edu
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

#ifdef __APPLE__
#include <SDL/SDL_main.h>
#include <SDL/SDL_opengl.h>
#else
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#endif

/* screen width, height, and bit depth */
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 16

/* Define our booleans */
#define TRUE 1
#define FALSE 0

int keys[1024];
static GLfloat rtri, rquad, rroll, zoom;

/* This is our SDL surface */
SDL_Surface *surface;

/* function to release/destroy our resources and restoring the old desktop */
void Quit(int returnCode) {
  /* clean up the window */
  SDL_Quit();

  /* and exit appropriately */
  exit(returnCode);
}

/* function to reset our viewport after a window resize */
int resizeWindow(int width, int height) {
  /* Height / width ration */
  GLfloat ratio;

  /* Protect against a divide by zero */
  if (height == 0)
    height = 1;

  ratio = (GLfloat)width / (GLfloat)height;

  /* Setup our viewport. */
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  /* change to the projection matrix and set our viewing volume. */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /* Set our perspective */
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);

  /* Make sure we're chaning the model view and not the projection */
  glMatrixMode(GL_MODELVIEW);

  /* Reset The View */
  glLoadIdentity();

  return (TRUE);
}

/* function to handle key press events */
void handleKeyPress(SDL_keysym *keysym) {
  //
  switch (keysym->sym) {
  case SDLK_ESCAPE:
    /* ESC key was pressed */
    Quit(0);
    break;
  case SDLK_F1:
    /* F1 key was pressed
     * this toggles fullscreen mode
     */
    SDL_WM_ToggleFullScreen(surface);
    break;
  default:
    break;
  }

  return;
}

/* general OpenGL initialization function */
int initGL(GLvoid) {

  /* Enable smooth shading */
  glShadeModel(GL_SMOOTH);

  /* Set the background black */
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  /* Depth buffer setup */
  glClearDepth(1.0f);

  /* Enables Depth Testing */
  glEnable(GL_DEPTH_TEST);

  /* The Type Of Depth Test To Do */
  glDepthFunc(GL_LEQUAL);

  /* Really Nice Perspective Calculations */
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  return (TRUE);
}

/* Here goes our drawing code */
int drawGLScene(GLvoid) {
  SDL_Delay((1.0f / 60.0f) * 1024);

  /* rotational vars for the triangle and quad, respectively */

  /* These are to calculate our fps */
  static GLint T0 = 0;
  static GLint Frames = 0;

  /* Clear The Screen And The Depth Buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Move Left 1.5 Units And Into The Screen 6.0 */
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, zoom + -6.0f);

  /* Rotate The Triangle On The Y axis ( NEW ) */
  glRotatef(rtri, 1.0f, 0.0f, 0.0f);

  glRotatef(rquad, 0.0f, 1.0f, 0.0f);

  glRotatef(rroll, 0.0f, 0.0f, 1.0f);

  glBegin(GL_TRIANGLES);          /* Drawing Using Triangles       */
  glColor3f(1.0f, 0.0f, 0.0f);    /* Red                           */
  glVertex3f(0.0f, 1.0f, 0.0f);   /* Top Of Triangle (Front)       */
  glColor3f(0.0f, 1.0f, 0.0f);    /* Green                         */
  glVertex3f(-1.0f, -1.0f, 1.0f); /* Left Of Triangle (Front)      */
  glColor3f(0.0f, 0.0f, 1.0f);    /* Blue                          */
  glVertex3f(1.0f, -1.0f, 1.0f);  /* Right Of Triangle (Front)     */

  glColor3f(1.0f, 0.0f, 0.0f);    /* Red                           */
  glVertex3f(0.0f, 1.0f, 0.0f);   /* Top Of Triangle (Right)       */
  glColor3f(0.0f, 0.0f, 1.0f);    /* Blue                          */
  glVertex3f(1.0f, -1.0f, 1.0f);  /* Left Of Triangle (Right)      */
  glColor3f(0.0f, 1.0f, 0.0f);    /* Green                         */
  glVertex3f(1.0f, -1.0f, -1.0f); /* Right Of Triangle (Right)     */

  glColor3f(1.0f, 0.0f, 0.0f);     /* Red                           */
  glVertex3f(0.0f, 1.0f, 0.0f);    /* Top Of Triangle (Back)        */
  glColor3f(0.0f, 1.0f, 0.0f);     /* Green                         */
  glVertex3f(1.0f, -1.0f, -1.0f);  /* Left Of Triangle (Back)       */
  glColor3f(0.0f, 0.0f, 1.0f);     /* Blue                          */
  glVertex3f(-1.0f, -1.0f, -1.0f); /* Right Of Triangle (Back)      */

  glColor3f(1.0f, 0.0f, 0.0f);     /* Red                           */
  glVertex3f(0.0f, 1.0f, 0.0f);    /* Top Of Triangle (Left)        */
  glColor3f(0.0f, 0.0f, 1.0f);     /* Blue                          */
  glVertex3f(-1.0f, -1.0f, -1.0f); /* Left Of Triangle (Left)       */
  glColor3f(0.0f, 1.0f, 0.0f);     /* Green                         */
  glVertex3f(-1.0f, -1.0f, 1.0f);  /* Right Of Triangle (Left)      */
  glEnd();                         /* Finished Drawing The Triangle */

  /* Draw it to the screen */
  SDL_GL_SwapBuffers();

  /* Gather our frames per second */
  Frames++;
  {
    GLint t = SDL_GetTicks();
    if (t - T0 >= 5000) {
      GLfloat seconds = (t - T0) / 1000.0;
      GLfloat fps = Frames / seconds;
      printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
      T0 = t;
      Frames = 0;
    }
  }

  // printf("%d %d %d %d\n", keys[119], keys[115], keys[97], keys[100]);

  if (keys[119]) {
    rtri += 1.5f;
  }

  if (keys[115]) {
    rtri -= 1.5f;
  }

  if (keys[97]) {
    rquad += 1.5f;
  }

  if (keys[100]) {
    rquad -= 1.5f;
  }

  if (keys[113]) {
    rroll += 1.5f;
  }

  if (keys[101]) {
    rroll -= 1.5f;
  }

  if (keys[114]) {
    zoom += 0.1f;
  }

  if (keys[102]) {
    zoom -= 0.1f;
  }

  // rquad += 0.1f;

  /* Increase The Rotation Variable For The Triangle ( NEW ) */
  /*rtri  += 0.2f;*/
  /* Decrease The Rotation Variable For The Quad     ( NEW ) */
  /*rquad -=0.15f;*/

  return (TRUE);
}

/*int main( int argc, char **argv )*/
int main(int argc, char *argv[]) {
  for (int i = 0; i < 1024; i++) {
    keys[i] = 0;
  }
  /* Flags to pass to SDL_SetVideoMode */
  int videoFlags;
  /* main loop variable */
  int done = FALSE;
  /* used to collect events */
  SDL_Event event;
  /* this holds some info about our display */
  const SDL_VideoInfo *videoInfo;
  /* whether or not the window is active */
  int isActive = TRUE;

  /* initialize SDL */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
    Quit(1);
  }

  SDL_WM_SetCaption("lesson05", "lesson05");

  /* Fetch the video info */
  videoInfo = SDL_GetVideoInfo();

  if (!videoInfo) {
    fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
    Quit(1);
  }

  /* the flags to pass to SDL_SetVideoMode */
  videoFlags = SDL_OPENGL;           /* Enable OpenGL in SDL */
  videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
  videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
  videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

  /* This checks to see if surfaces can be stored in memory */
  if (videoInfo->hw_available)
    videoFlags |= SDL_HWSURFACE;
  else
    videoFlags |= SDL_SWSURFACE;

  /* This checks if hardware blits can be done */
  if (videoInfo->blit_hw)
    videoFlags |= SDL_HWACCEL;

  /* Sets up OpenGL double buffering */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  /* get a SDL surface */
  surface =
      SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);

  /* Verify there is a surface */
  if (!surface) {
    fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
    Quit(1);
  }

  /* initialize OpenGL */
  initGL();

  /* resize the initial window */
  resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

  isActive = TRUE;

  /* wait for events */
  while (!done) {
    /* handle the events in the queue */

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_ACTIVEEVENT:
        /* Something's happend with our focus
         * If we lost focus or we are iconified, we
         * shouldn't draw the screen
         */
        /*
       if ( event.active.gain == 0 )
           isActive = FALSE;
       else
           isActive = TRUE;
       break;
       */
        break;
      case SDL_VIDEORESIZE:
        /* handle resize event */
        surface =
            SDL_SetVideoMode(event.resize.w, event.resize.h, 16, videoFlags);
        if (!surface) {
          fprintf(stderr, "Could not get a surface after resize: %s\n",
                  SDL_GetError());
          Quit(1);
        }
        resizeWindow(event.resize.w, event.resize.h);
        break;
      case SDL_KEYDOWN:
        /* handle key presses */
        handleKeyPress(&event.key.keysym);

        int kid = (int)event.key.keysym.sym;

        if (kid >= 0 && kid < 1024) {
          keys[kid] = 1;
        }
        printf("*** KEY DOWN %d\n", event.key.keysym.sym);
        break;
      case SDL_KEYUP:

        kid = (int)event.key.keysym.sym;

        if (kid >= 0 && kid < 1024) {
          keys[kid] = 0;
        }

        printf("*** KEY UP %d\n", event.key.keysym.sym);

        break;
      case SDL_QUIT:
        /* handle quit requests */
        done = TRUE;
        break;
      default:
        break;
      }
    }

    /* draw the scene */

    drawGLScene();
  }

  /* clean ourselves up and exit */
  Quit(0);

  /* Should never get here */
  return (0);
}
