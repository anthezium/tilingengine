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
#include "tile.h"

Tile::Tile(int _x, int _y, int _layer, int texNum, double _direction){
  tile_t temp;
  x=_x;
  y=_y;
  layer=_layer;
  direction=_direction;
  
  temp.texName=texNum;
  temp.on=1;
  t = setTile(x,y,layer,temp);
}
