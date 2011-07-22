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
#include "mover.h"

Mover::Mover(int _x, int _y, int _layer, int texNum, double _direction) 
     : Tile(_x,_y,_layer,texNum,_direction) {}

void Mover::move(int dx, int dy, int dlayer){
  tile_t* temp;
  x+=dx;
  y+=dy;
  layer+=dlayer;
  temp=setTile(x,y,layer,*t);
  resetTile(t);
  t=temp;
}
