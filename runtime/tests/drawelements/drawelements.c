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
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <string.h>

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  1080
#define SCREEN_HEIGHT 720
#define SCREEN_BPP     16

/* Define our booleans */
#define TRUE  1
#define FALSE 0

static const GLenum FORMAT = GL_BGR;
static const GLuint FORMAT_NBYTES = 3;
static GLubyte *pixels = NULL;
static unsigned int nscreenshots = 0;
static unsigned int nvideos = 0;
/* This is our SDL surface */
SDL_Surface *surface;

/* function to release/destroy our resources and restoring the old desktop */
void Quit( int returnCode )
{
    /* clean up the window */
    SDL_Quit( );

    /* and exit appropriately */
    exit( returnCode );
}

/* function to reset our viewport after a window resize */
int resizeWindow( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;
 
    /* Protect against a divide by zero */
   if ( height == 0 )
	height = 1;

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );

    return( TRUE );
}

/* function to handle key press events */
void handleKeyPress( SDL_keysym *keysym )
{
    switch ( keysym->sym )
	{
	case SDLK_ESCAPE:
	    /* ESC key was pressed */
	    Quit( 0 );
	    break;
	case SDLK_F1:
	    /* F1 key was pressed
	     * this toggles fullscreen mode
	     */
	    SDL_WM_ToggleFullScreen( surface );
	    break;
	default:
	    break;
	}

    return;
}

/* general OpenGL initialization function */
int initGL( GLvoid )
{

    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );
    
    pixels = malloc(FORMAT_NBYTES * SCREEN_WIDTH * SCREEN_HEIGHT);
   
   // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /* Depth buffer setup */
    glClearDepth( 1.0f );

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    return( TRUE );
}









// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// vertex coords array
GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
                      1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
                      1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
                      -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
                      -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
                      1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

// normal array
GLfloat normals[] = {0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
                     1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
                     0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
                     -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
                     0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
                     0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5

// color array
GLfloat colors[] = {1,1,1,  1,1,0,  1,0,0,  1,0,1,              // v0-v1-v2-v3
                    1,1,1,  1,0,1,  0,0,1,  0,1,1,              // v0-v3-v4-v5
                    1,1,1,  0,1,1,  0,1,0,  1,1,0,              // v0-v5-v6-v1
                    1,1,0,  0,1,0,  0,0,0,  1,0,0,              // v1-v6-v7-v2
                    0,0,0,  0,0,1,  1,0,1,  1,0,0,              // v7-v4-v3-v2
                    0,0,1,  0,0,0,  0,1,0,  0,1,1};             // v4-v7-v6-v5

// index array of vertex array for glDrawElements()
// Notice the indices are listed straight from beginning to end as exactly
// same order of vertex array without hopping, because of different normals at
// a shared vertex. For this case, glDrawArrays() and glDrawElements() have no
// difference.
GLubyte indices[] = {0,1,2,3,
                     4,5,6,7,
                     8,9,10,11,
                     12,13,14,15,
                     16,17,18,19,
                     20,21,22,23};


///////////////////////////////////////////////////////////////////////////////
// draw 1, immediate mode
// 54 calls = 24 glVertex*() calls + 24 glColor*() calls + 6 glNormal*() calls
///////////////////////////////////////////////////////////////////////////////
void draw1()
{
    glPushMatrix();
    //glTranslatef(-2, 2, 0); // move to upper left corner
    glBegin(GL_QUADS);
        // face v0-v1-v2-v3
        glNormal3f(0,0,1);
        glColor3f(1,1,1);
        glVertex3f(1,1,1);
        glColor3f(1,1,0);
        glVertex3f(-1,1,1);
        glColor3f(1,0,0);
        glVertex3f(-1,-1,1);
        glColor3f(1,0,1);
        glVertex3f(1,-1,1);

        // face v0-v3-v4-v6
        glNormal3f(1,0,0);
        glColor3f(1,1,1);
        glVertex3f(1,1,1);
        glColor3f(1,0,1);
        glVertex3f(1,-1,1);
        glColor3f(0,0,1);
        glVertex3f(1,-1,-1);
        glColor3f(0,1,1);
        glVertex3f(1,1,-1);

        // face v0-v5-v6-v1
        glNormal3f(0,1,0);
        glColor3f(1,1,1);
        glVertex3f(1,1,1);
        glColor3f(0,1,1);
        glVertex3f(1,1,-1);
        glColor3f(0,1,0);
        glVertex3f(-1,1,-1);
        glColor3f(1,1,0);
        glVertex3f(-1,1,1);

        // face  v1-v6-v7-v2
        glNormal3f(-1,0,0);
        glColor3f(1,1,0);
        glVertex3f(-1,1,1);
        glColor3f(0,1,0);
        glVertex3f(-1,1,-1);
        glColor3f(0,0,0);
        glVertex3f(-1,-1,-1);
        glColor3f(1,0,0);
        glVertex3f(-1,-1,1);

        // face v7-v4-v3-v2
        glNormal3f(0,-1,0);
        glColor3f(0,0,0);
        glVertex3f(-1,-1,-1);
        glColor3f(0,0,1);
        glVertex3f(1,-1,-1);
        glColor3f(1,0,1);
        glVertex3f(1,-1,1);
        glColor3f(1,0,0);
        glVertex3f(-1,-1,1);

        // face v4-v7-v6-v5
        glNormal3f(0,0,-1);
        glColor3f(0,0,1);
        glVertex3f(1,-1,-1);
        glColor3f(0,0,0);
        glVertex3f(-1,-1,-1);
        glColor3f(0,1,0);
        glVertex3f(-1,1,-1);
        glColor3f(0,1,1);
        glVertex3f(1,1,-1);
    glEnd();

    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// draw cube at upper-right corner with glDrawArrays
///////////////////////////////////////////////////////////////////////////////
void draw2()
{
    // enble and specify pointers to vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glPushMatrix();
    //glTranslatef(2, 2, 0);                  // move to upper-right

    glDrawArrays(GL_QUADS, 0, 24);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

///////////////////////////////////////////////////////////////////////////////
// draw cube at bottom-left corner with glDrawElements
// In this example, glDrawElements() has no advantage over glDrawArrays(),
// because the shared vertices cannot share normals, so they must be duplicated
// once per face. Look at the index array defined earlier in this file. The
// indices are marching straight from 0 to 23 without hopping.
///////////////////////////////////////////////////////////////////////////////
void draw3()
{
    // enable and specify pointers to vertex arrays
    //glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    //glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glPushMatrix();
    //glTranslatef(-2, -2, 0);                // move to bottom-left

    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
}

///////////////////////////////////////////////////////////////////////////////
// draw cube at bottom-right corner with glDrawRangeElements()
// glDrawRangeElements() has two more parameters compared with glDrawElements(),
// start and end index. The values in index array must lie in between start and
// end. Note that not all vertices in range (start, end) must be referenced.
// But, if you specify a sparsely used range, it causes unnecessary process for
// many unused vertices in that range.
///////////////////////////////////////////////////////////////////////////////
void draw4()
{
    // enable and specify pointers to vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glPushMatrix();
    //glTranslatef(2, -2, 0);                 // move to bottom-right

    // draw first half (12 elements) {0,1,2,3, 4,5,6,7, 8,9,10,11}
    // tell the driver we use vertices from index 0 to index 11, which means 11-0+1 = 12 vertices
    // So, the driver can prefetch an amount of 12 vertex data prior to rendering
    glDrawRangeElements(GL_QUADS, 0, 11, 12, GL_UNSIGNED_BYTE, indices);

    // draw last half (12 elements) {12,13,14,15, 16,17,18,19, 20,21,22,23}
    // tell the driver we use vertices from index 12 to index 23, which means 23-12+1 = 12 vertices
    // So, the driver can prefetch an amount of 12 vertex data prior to rendering
    glDrawRangeElements(GL_QUADS, 12, 23, 12, GL_UNSIGNED_BYTE, indices + 12);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static void create_ppm(char *prefix, int frame_id, unsigned int width, unsigned int height,
        unsigned int color_max, unsigned int pixel_nbytes, GLubyte *pixels) {

    enum Constants { max_filename = 256, max_videoname=256 };
    char filename[max_filename];
    char videoname[max_videoname];
    snprintf(filename, max_filename, "%s%d.bmp", prefix, frame_id);
    unsigned char bmp_file_header[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, };
	unsigned char bmp_info_header[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, };
	unsigned char bmp_pad[3] = { 0, 0, 0 };

	const int size = 54 + width * height * pixel_nbytes;

	bmp_file_header[2] = (unsigned char)size;
	bmp_file_header[3] = (unsigned char)(size >> 8);
	bmp_file_header[4] = (unsigned char)(size >> 16);
	bmp_file_header[5] = (unsigned char)(size >> 24);

	bmp_info_header[4] = (unsigned char)(width);
	bmp_info_header[5] = (unsigned char)(width >> 8);
	bmp_info_header[6] = (unsigned char)(width >> 16);
	bmp_info_header[7] = (unsigned char)(width >> 24);

	bmp_info_header[8] = (unsigned char)(height);
	bmp_info_header[9] = (unsigned char)(height >> 8);
	bmp_info_header[10] = (unsigned char)(height >> 16);
        bmp_info_header[11] = (unsigned char)(height >> 24);
        FILE *f = fopen(filename, "wb");
        if (f)
	{
		fwrite(bmp_file_header, 1, 14, f);
		fwrite(bmp_info_header, 1, 40, f);

		for (int i = 0; i < height; i++)
		// for (int i = (height - 1); i >= 0; i--)
		{
			fwrite(pixels + (width * i * 3), 3, width, f);
			fwrite(bmp_pad, 1, ((4 - (width * 3) % 4) % 4), f);
		}

		fclose(f);

		//return;
	}
        if(frame_id>=100)
        {
           frame_id=-1;
           printf("%s\n","Hello!!!");
           nvideos++;
           snprintf(videoname, max_videoname, "%s%d.mov", "out", nvideos);
           char* command = concat("./ffmpeg.sh ",videoname);
           system(command);
           
        }
        
        while(frame_id==-1&&access(videoname, F_OK ) == -1);
        
        return;
}



/* Here goes our drawing code */
int drawGLScene( GLvoid )
{
	SDL_Delay((1.0f / 60.0f) * 1024);


    /* rotational vars for the triangle and quad, respectively */
    /*static GLfloat rtri, rquad;*/
    static GLfloat rtri, rquad;
    /* These are to calculate our fps */
    static GLint T0     = 0;
    static GLint Frames = 0;
    
    
    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
    //glTranslatef( 2, -2.0f, -8.0f );
    glTranslatef( 0, 0, -8.0f );
    
    glRotatef( rtri, 0.0f, 1.0f, 0.0f );
    glRotatef( rquad, 1.0f, 0.0f, 1.0f );
    
	draw1();
   
    /* Draw it to the screen */
    SDL_GL_SwapBuffers( );
    //glutSwapBuffers();
    glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FORMAT, GL_UNSIGNED_BYTE, pixels);
    
     //for(int i=0;i<8;i++)
      //printf("%u\n",strlen(pixels));
    //create_ppm("tmp", nscreenshots, SCREEN_WIDTH, SCREEN_HEIGHT, 255, 4, pixels);
    if(nscreenshots>=100)
      nscreenshots=0;
    else
     nscreenshots++;

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

    /* Increase The Rotation Variable For The Triangle ( NEW ) */
    rtri  += 0.2f;
    /* Decrease The Rotation Variable For The Quad     ( NEW ) */
    rquad -=0.15f;

    return( TRUE );
}

int main( int argc, char **argv )
{
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
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* get a SDL surface */
    surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
				videoFlags );

    /* Verify there is a surface */
    if ( !surface )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    Quit( 1 );
	}

    /* initialize OpenGL */
    initGL( );

    /* resize the initial window */
    resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );
    
    isActive = TRUE;

    /* wait for events */ 
    while ( !done )
   	{
	 

  	    while ( SDL_PollEvent( &event ) )
		{
		    switch( event.type )
			{
			case SDL_QUIT:
			   
			    //done = TRUE;
                            SDL_Quit();
                            getchar(); 
			    break;
			default:
			    break;
			}
		}

	    /* draw the scene */
	    if ( isActive )
		drawGLScene( );
	}
   
    /* clean ourselves up and exit */
    Quit( 0 );
    /* Should never get here */
    return( 0 );
}
