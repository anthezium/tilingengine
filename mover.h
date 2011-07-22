#ifndef H_MOVER
#define H_MOVER

#include "tile.h"

class Mover : public Tile {
  public:
    //constructor
    Mover(int _x, int _y, int _layer, int texNum, double _direction=0.0);
    //mutators
    virtual void move(int dx, int dy, int dz);
};

#endif
