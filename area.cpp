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
#include "area.h"
#include "constants.h"
#include "tile.h"
#include <stdlib.h>

using namespace std;

Area::Area(int w, int h) 
    : shaker(6,6,0,1)
{
  int i,j,k;
  
  width=w;
  height=h;
  layers=NLAYERS;
  
  //allocate and zero obstacle map
  obstacleMap = (int***)malloc(layers*sizeof(int**));
  for(i=0;i<layers;i++){
    obstacleMap[i]=(int**)malloc(height*sizeof(int*));
    for(j=0;j<height;j++){
      obstacleMap[i][j]=(int*)malloc(width*sizeof(int));
      bzero(obstacleMap[i][j], sizeof(int)*width);
    }
  }
  
  //area initialization... eventually load this from a file
  //grass tiles on layer 1
  for(i=0;i<width;i++)
    for(j=0;j<height;j++){
      tiles.push_back(Tile(i,j,1,2));
    }
    
  //happy man on layer 0!
  tiles.push_back(shaker);
  
  //iguana obstacles
  iguanaObstacle(0,7);
  iguanaObstacle(13,4);
}

//private functions

void Area::iguanaObstacle(int llx, int lly){
  int i,j;
  
  //iguana obstacle on layer 0
  for(i=3;i>=0;i--)
    for(j=0;j<4;j++){
      Tile(j+llx,i+lly,0,(4*(3-i) + j)+3); 
      setObstacle(j+llx,i+lly,0);
    }
}

//public functions

Area::~Area(){
  int i,j;
  
  for(i=0;i<layers;i++){
    for(j=0;j<height;j++)
      free(obstacleMap[i][j]);
    free(obstacleMap[i]);
  }
  free(obstacleMap);
}

inline void Area::setObstacle(int x, int y, int z){
  obstacleMap[z][y][x]=1;
}

inline void Area::resetObstacle(int x, int y, int z){
  obstacleMap[z][y][x]=0;
}

bool Area::canGo(Mover& m, int x, int y, int z){
  //is it inside the area?
  if( 0 <= x && x < width &&
      0 <= y && y < height &&
      0 <= z && z < layers ){
    //is it not an obstacle?
    if(!obstacleMap[z][y][x])
      return true;
  }
  return false;
}

void Area::moveShaker(int dx, int dy, int dz){
  //is it within the area?
  if(canGo(shaker, shaker.x+dx, shaker.y+dy, shaker.layer+dz)){
    if(moveFocus(dx,dy, width, height)){
      shaker.move(dx, dy, dz);
    }  
  }
}

Mover& Area::getShaker(){
  return shaker;
}
