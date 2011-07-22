#ifndef H_HANDLER
#define H_HANDLER

#include "area.h"
#include "mover.h"

using namespace std;

typedef enum { NOKEY, UPKEY, DOWNKEY, LEFTKEY, RIGHTKEY } keyevent_t;

class Handler {
  private:
    Area& a;
  public:
    Handler(Area& _a);
    void specialkey(keyevent_t key, int mouseX, int mouseY);
};

#endif
