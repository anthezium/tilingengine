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
#include "handler.h"

Handler::Handler(Area& _a)
       : a(_a) {}

void Handler::specialkey(keyevent_t key, int mouseX, int mouseY){
  switch(key){
    case LEFTKEY : a.moveShaker(-1,0,0); break;
    case RIGHTKEY : a.moveShaker(1,0,0); break;
    case UPKEY : a.moveShaker(0,1,0); break;
    case DOWNKEY : a.moveShaker(0,-1,0); break;
  }
  
}
