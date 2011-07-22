#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tile.h"
#include "engine.h" //public interface
//macros
#define EITHERCASE(key, c) (key) == (c) || (key) == ((char)((c) - 32))

//globals
#define NTEXTURES 128
#define NLAYERS 10
#define TEXW 64
#define TEXH 64
//12x12 plane for now
#define GRID 12

//bitmaps
static char* bitmaps[] = { "textures/happy.bmp",
                           "textures/grass.bmp",
                           "textures/iguana1.bmp" };
static int nBitmaps = 3; //must be less than NTEXTURES

//textures
static GLuint texNames[NTEXTURES];
static int texcount = 0;

//world dimensions
const GLdouble wXMin = -6.0;
const GLdouble wXMax =  6.0;
const GLdouble wYMin = -6.0;
const GLdouble wYMax =  6.0;
const GLdouble far   =  10.0;
const GLdouble near  =  0.0;

//window dimensions
const GLint winW = TEXW*GRID;
const GLint winH = TEXH*GRID;

//tiles
tile_t tiles[GRID][GRID][NLAYERS]; //texNames
GLdouble tileW;
GLdouble tileH;

//members
Mover moveyDude;

//forward declarations
int loadBitmap(char* szFileName, GLuint texid, int width, int height);
void keyboard(unsigned char key, int mouseX, int mouseY);
void init3D();
void display();
void loadTextures();
void fillTiles();
void specialkey(int key, int mouseX, int mouseY);

//MAIN MAN

//------------------------------------------------------------
int main(int argc, char* argv[]) {
  //attend to globals
  tileW = (wXMax - wXMin)/GRID;
  tileH = (wYMax - wYMin)/GRID;

  // initialize window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(winW, winH);
  glutInitWindowPosition(100,150);
  glutCreateWindow("Look ma, tiles!");

  // register callback functions
  glutDisplayFunc(display);
  //glutMouseFunc(mouseclick);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialkey);

  init3D();  //basic OpenGL initializations
  loadTextures(); //load textures from bitmaps
  fillTiles();
  glutMainLoop();
  return 0;
}

//INITIALIZATION

//--------------------------
// basic 3D initializations
//--------------------------
void init3D() {
  //turn on 3D-related processing
  glEnable(GL_DEPTH_TEST);//enable depth testing
  glDepthMask(GL_TRUE);
  //glEnable(GL_LIGHTING);  //enable OpenGL lighting routines
  //glEnable(GL_LIGHT0);    //turn on default light
  //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_BLEND);
  //glAlphaFunc(GL_GREATER, 0.6);
  //glEnable(GL_ALPHA_TEST);
  

  glClearColor(1.0, 1.0, 1.0, 1.0); //black background
  glViewport(0, 0, winW, winH);     //set size of viewport (in pixels)
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(NTEXTURES, texNames);
  
  //specify view volume extent (Note: Projection coordinates must
  // be given in camera coords. For this program, the camera will
  // be at the origin, so camera coords = world coords.)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(-worldW/2, worldW/2, -worldH/2, worldH/2, near, near+worldD);
  glOrtho(wXMin, wXMax, wYMin, wYMax, near, far);

  //clear modelview matrix and set it to be the current matrix (so it is
  // current when the display routines execute).
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //set camera position/orientation here so viewing transformation is the
  // first thing loaded into the modelview matrix. 
  gluLookAt(0.0, 0.0, 0.0, 
            0.0, 0.0, -1.0,
            0.0, 1.0, 0.0); //default settings for now
}

void fillTiles(){
  int i,j;
  tile_t t;  
  
  //grass tiles on layer 1
  for(i=0;i<GRID;i++)
    for(j=0;j<GRID;j++){
      t.on = 1;
      t.texName = texNames[1];
      tiles[i][j][1] = t;
    }
    
  //happy man on layer 0!
  t.on = 1;
  t.texName = texNames[2];
  tiles[6][6][0] = t;
  moveyDude = &tiles[6][6][0];
}

//TEXTURE PROCESSING FUNCTIONS

void loadTextures(){
  int i;
  
  for(i=0;i<nBitmaps;i++){
    if(i > NTEXTURES - 1){
      perror("Ran out of room for textures... won't read any more\n");
      break;
    }
    loadBitmap(bitmaps[i], texNames[i], TEXW, TEXH);
    texcount++;
  }
}

int loadBitmap(char* szFileName, GLuint texid, int width, int height)
// Creates Texture From A Bitmap File
{
  GLubyte tex[width*height*4 + 18*3]; //array to hold data input
  GLubyte tex2[width*height*4]; //array to hold texture data
  GLubyte temp; //swap variable
  int i; //loop control variable
  FILE * f; //open the bitmap for reading

  if ((f=fopen(szFileName,"r"))== NULL){
    return 1;
  }

  fread(tex,width*height*4 + 18*3,1,f); //read data from bitmap
  fclose(f); //close the file

  for (i = 18*3; i < width*height*4 + 18*3; i++){
    //remove the first 18 pixels, RGBA code = 4 bytes
    tex2[i - 18*3] = tex[i]; //B
  }

  for (i = 0; i < width*height*4; i += 4){
    //reorder ABGR to RGBA
    /*temp = tex2[i];
    tex2[i] = tex2[i + 3];
    tex2[i + 3] = temp;
    
    temp = tex2[i + 1];
    tex2[i + 1] = tex2[i + 2];
    tex2[i + 2] = temp;
    */
    //reorder ARGB to RGBA
    GLubyte r,g,b,a;
    b=tex2[i];
    g=tex2[i+1];
    r=tex2[i+2];
    a=tex2[i+3];
    
    tex2[i]=r;
    tex2[i+1]=g;
    tex2[i+2]=b;
    tex2[i+3]=a;
    
    printf("texture values (%d,%d,%d,%d)\n",tex2[i],tex2[i+1],tex2[i+2],tex2[i+3]);
  }

  
  
  glBindTexture(GL_TEXTURE_2D, texid); //select ID to store the texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0,3, width ,height, 0,GL_RGBA, GL_UNSIGNED_BYTE, tex2);
  //produce the texture
  return texid;
}

//EVENT HANDLING

//--------------------------
// function: keyboard
// catches keyboard events
//--------------------------
void keyboard(unsigned char key, int mouseX, int mouseY) {
  if(key == 27 || EITHERCASE(key,'q')){ //quit on q or ESC
    exit(0);
  }
  glutPostRedisplay();
}

//--------------------------
// function: specialkey
// catches keyboard events without ASCII codes
//--------------------------
void specialkey(int key, int mouseX, int mouseY){
  switch(key){
    case GLUT_KEY_LEFT :  
     if(moveyDude > 0)
      moveyDude->x--;
     break;
    case GLUT_KEY_RIGHT : 
      if(moveyDude->x < GRID - 1)
        moveyDude->x++;
      break;
    case GLUT_KEY_UP : 
      if(moveyDude->y < GRID - 1)
        moveyDude->y++;
      break;
    case GLUT_KEY_DOWN : 
      if(moveyDude->y > 0)
        moveyDude->y--;
      break;
  }
  
  glutPostRedisplay();
}

//DISPLAY FUNCTIONS

//--------------------------
// function: display
//--------------------------
void display() {
  int i,j,k;
  //clear the frame buffer to begin a new frame
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  //tiles
  glEnable(GL_TEXTURE_2D);
  for(i=0;i<GRID;i++){
    for(j=0;j<GRID;j++){
      for(k=0;k<LAYERS;k++){
        if(tiles[i][j][k].on){
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          glBindTexture(GL_TEXTURE_2D, tiles[i][j][k].texName);

          glBegin(GL_POLYGON);
            glColor4d(1.0,1.0,1.0,1.0);
            glTexCoord2f(0.0, 0.0); 
            glVertex3f(wXMin + tileW*i, 
                       wYMin + tileH*j, 
                       -1.0*(GLdouble)k);
            glTexCoord2f(1.0, 0.0); 
            glVertex3f(wXMin + tileW*(i + 1), 
                       wYMin + tileH*j, 
                       -1.0*(GLdouble)k);
            glTexCoord2f(1.0, 1.0); 
            glVertex3f(wXMin + tileW*(i + 1), 
                       wYMin + tileH*(j + 1), 
                       -1.0*(GLdouble)k);
            glTexCoord2f(0.0, 1.0); 
            glVertex3f(wXMin + tileW*i, 
                       wYMin + tileH*(j + 1), 
                       -1.0*(GLdouble)k);  
          glEnd();
        }  
      }
    }
  }
  glFlush();
  glDisable(GL_TEXTURE_2D);
    
  //detect rendering errors (note that the call to glGetError() clears
  // the error condition too, so you must call it and save the error code).
  int error = glGetError();
  if (error != GL_NO_ERROR)
    printf("OpenGL Error: %s\n", gluErrorString(error));

  glutSwapBuffers();
}
