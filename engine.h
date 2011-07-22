#ifndef H_ENGINE
#define H_ENGINE

#include "tile_t.h"

tile_t* setTile(int i, int j, int k, tile_t t);
void resetTile(tile_t* t);
bool moveFocus(int dx, int dy, int aWidth, int aHeight);

#endif

