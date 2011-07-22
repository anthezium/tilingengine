#ifndef H_TILE
#define H_TILE

#include "engine.h"
#include "tile_t.h"

class Tile {
  protected:
    tile_t* t;
  public:
    int x,y,layer;
    double direction;
    Tile(int _x, int _y, int _layer, int texNum, double _direction=0.0);
};

#endif
