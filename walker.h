#ifndef H_WALKER
#define H_WALKER

#include "mover.h"

class Walker : public Mover {
  public:
    //constructor
    Walker(int _x, int _y, int _layer, int texNum, double _direction=0.0);
    
    //mutator
    virtual void move(int dx, int dy, int dz);

};

#endif
