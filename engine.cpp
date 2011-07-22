/*
* License:  This  program  is  free  software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published by
* the  Free Software Foundation; either version 3 of the License, or (at your
* option)  any later version. This program is distributed in the hope that it
* will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
* Public License for more details.
*
* Copyright Ted Cooper, 2011
*/
#include <vector>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tile_t.h"
#include "tile.h"
#include "mover.h"
#include "area.h"
#include "engine.h" //public interface
#include "handler.h"
#include "constants.h"


//macros
#define EITHERCASE(key, c) (key) == (c) || (key) == ((char)((c) - 32))

//bitmaps
static char* bitmaps[] = { "textures/happy.bmp",
                           "textures/grass.bmp",
                           "textures/iguana0.bmp",
                           "textures/iguana1.bmp",
                           "textures/iguana2.bmp",
                           "textures/iguana3.bmp",
                           "textures/iguana4.bmp",
                           "textures/iguana5.bmp",
                           "textures/iguana6.bmp",
                           "textures/iguana7.bmp",
                           "textures/iguana8.bmp",
                           "textures/iguana9.bmp",
                           "textures/iguana10.bmp",
                           "textures/iguana11.bmp",
                           "textures/iguana12.bmp",
                           "textures/iguana13.bmp",
                           "textures/iguana14.bmp",
                           "textures/iguana15.bmp",
                            };
static int nBitmaps = 18; //must be less than NTEXTURES

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
tile_t tiles[NLAYERS][MAXAREAH][MAXAREAW]; //texNames
GLdouble tileW;
GLdouble tileH;

//objects
Area* area;
Handler* handler;
Tile* tileObjs[1024];
int nTileObjs = 0;

//view
int focusX;
int focusY;
int dispX=0;
int dispY=0;

//forward declarations
int loadBitmap(char* szFileName, GLuint texid, int width, int height);
void keyboard(unsigned char key, int mouseX, int mouseY);
void init3D();
void display();
void loadTextures();
void specialkey(int key, int mouseX, int mouseY);

//MAIN MAN

//------------------------------------------------------------
int main(int argc, char* argv[]) {
  int i;
  
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
  
  //construct objects
  area = new Area(GRID*4, GRID*4);
  focusX=area->getShaker().x;
  focusY=area->getShaker().y;
  
  handler = new Handler(*area);
  
  glutMainLoop();
  
  delete handler;
  delete area;
  
  return 0;
}

//PUBLIC FUNCTIONS
extern tile_t* setTile(int x, int y, int layer, tile_t t){
  tiles[layer][y][x]=t;
  return &tiles[layer][y][x];
}

extern void resetTile(tile_t* t){
  t->on=0;
}

//NOTE: as currently written, this will probably break when both
//dx and dy are nonzero
extern bool moveFocus(int dx, int dy, int aWidth, int aHeight){
  int newX = focusX + dx;
  int newY = focusY + dy;
  //new position relative to current display window
  int relX = newX - dispX;
  int relY = newY - dispY;
  
  //is the new display position within the bounds of the tiles data structure?
  if(0 <= dispX + dx && dispX + dx <= aWidth - GRID &&
     0 <= dispY + dy && dispY + dy <= aHeight - GRID){
    printf("moveFocus: inside first. dx: %d, dy: %d, newX: %d, newY: %d, relX: %d, relY: %d\n",dx,dy,newX,newY,relX,relY);
    //has the position changed enough that the entire view should be moved?
    //(if not, only the focus should be changed)
    if((dx < 0 && relX == GRID/3-1) || (dx > 0 && relX == 2*GRID/3) || dx > 3){
      printf("disp should shift left or right\n");
      dispX+=dx;
      //xshift=true;
    }
    
    if((dy < 0 && relY == GRID/3-1) || (dy > 0 && relY == 2*GRID/3) || dy > 3){
      printf("disp should shift up or down\n");
      dispY+=dy; 
      //yshift=true;
    }
    
    focusX+=dx;
    focusY+=dy;
    
    glutPostRedisplay();
    return true;
  }
  
  //edge of area cases
  if(dx != 0 && 0 <= focusX + dx && focusX + dx < aWidth){
    printf("moveFocus: inside second. dx: %d, dy: %d, newX: %d, newY: %d, relX: %d, relY: %d, aWidth: %d\n",dx,dy,newX,newY,relX,relY,aWidth); 
    focusX+=dx;
    glutPostRedisplay();
    return true;
  }
  if(dy != 0 && 0 <= focusY + dy && focusY + dy < aHeight){
    printf("moveFocus: inside third. dx: %d, dy: %d, newX: %d, newY: %d, relX: %d, relY: %d, aHeight: %d\n\n",dx,dy,newX,newY,relX,relY,aHeight);
    focusY+=dy;
    glutPostRedisplay();
    return true;
  }
  
  return false;
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
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_ALPHA);
  glEnable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
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
    
    //printf("texture values (%d,%d,%d,%d)\n",tex2[i],tex2[i+1],tex2[i+2],tex2[i+3]);
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
  keyevent_t hkey=NOKEY;
  switch(key){
    case GLUT_KEY_LEFT : hkey=LEFTKEY; break;
    case GLUT_KEY_RIGHT : hkey=RIGHTKEY; break;
    case GLUT_KEY_UP : hkey=UPKEY; break;
    case GLUT_KEY_DOWN : hkey=DOWNKEY; break;
  }  
  handler->specialkey(hkey,mouseX,mouseY);
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
  for(i=0;i<NLAYERS;i++){
    for(j=0;j<GRID;j++){
      for(k=0;k<GRID;k++){
        if(tiles[i][j+dispY][k+dispX].on){
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          glBindTexture(GL_TEXTURE_2D, tiles[i][j+dispY][k+dispX].texName);

          glBegin(GL_POLYGON);
            //glColor4d(1.0,1.0,1.0,1.0);
            glTexCoord2f(0.0, 0.0); 
            glVertex3f(wXMin + tileW*(k), 
                       wYMin + tileH*(j), 
                       -1.0*(GLdouble)i);
            glTexCoord2f(1.0, 0.0); 
            glVertex3f(wXMin + tileW*(k + 1), 
                       wYMin + tileH*(j), 
                       -1.0*(GLdouble)i);
            glTexCoord2f(1.0, 1.0); 
            glVertex3f(wXMin + tileW*(k + 1), 
                       wYMin + tileH*(j + 1), 
                       -1.0*(GLdouble)i);
            glTexCoord2f(0.0, 1.0); 
            glVertex3f(wXMin + tileW*(k), 
                       wYMin + tileH*(j + 1), 
                       -1.0*(GLdouble)i);  
          glEnd();
        }  
      }
    }
  }
  //glFlush();  //only necessary when not using double buffering
  glDisable(GL_TEXTURE_2D);
    
  //detect rendering errors (note that the call to glGetError() clears
  // the error condition too, so you must call it and save the error code).
  int error = glGetError();
  if (error != GL_NO_ERROR)
    printf("OpenGL Error: %s\n", gluErrorString(error));

  glutSwapBuffers();
}
