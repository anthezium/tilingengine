#ifndef H_AREA
#define H_AREA

#include <vector>
#include <strings.h>
#include "tile.h"
#include "mover.h"
#include "engine.h"

using namespace std;

class Area {

  private:
    int width,height,layers;
    vector<Tile> tiles;
    Mover shaker; 
    int*** obstacleMap;  
    
    inline void setObstacle(int x, int y, int z);
    inline void resetObstacle(int x, int y, int z);
    void iguanaObstacle(int llx, int lly);
  
  public:
    Area(int w, int h);
    ~Area();
    bool canGo(Mover& m, int x, int y, int z);
    void moveShaker(int dx, int dy, int dz);
    Mover& getShaker();

};

#endif
