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
#include "walker.h"

Walker::Walker(int _x, int _y, int _layer, int texNum, double _direction)
      : Mover(_x,_y,_layer,texNum,_direction) {}

void Walker::move(int dx, int dy, int layer){
  if(dx == 0 && dy != 0){
    direction = ( dy > 0 ? 90.0 : 270.0 );
  }
  else if(dx != 0 && dy == 0){
    direction = ( dx > 0 ? 0.0 : 180.0 );
  }
  Mover::move(dx, dy, layer);
}
