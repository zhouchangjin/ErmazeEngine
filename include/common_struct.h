#ifndef COMMON_STRUCT_H_INCLUDED
#define COMMON_STRUCT_H_INCLUDED

#include <vector>

namespace ge_common_struct{

  enum layer_type{
     OBJECT_LAYER,
     TILED_LAYER,
     PICTURE_LAYER
  };

  enum game_event{
    NO_EVENT,
    QUIT,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_SPACE,
    KEY_ESC,
    KEY_ENTER

  };

  struct ge_rect{
    int x;
    int y;
    int w;
    int h;
  };

  struct ge_color{
    int r;
    int g;
    int b;
  };

  typedef std::vector<int> ROW_IDX;

  typedef std::vector<ROW_IDX> LAYER_IDX;

}


#endif // COMMON_STRUCT_H_INCLUDED
