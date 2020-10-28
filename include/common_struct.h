#ifndef COMMON_STRUCT_H_INCLUDED
#define COMMON_STRUCT_H_INCLUDED

#include <vector>
#include <string>

namespace ge_common_struct{

  enum layer_type{
     OBJECT_LAYER,
     TILED_LAYER,
     PICTURE_LAYER
  };

  enum grid_type{
    NORMAL,
    EMPTY,
    COLLISION,
    LADDER,
    DOOR,
    TRANSFER,
    CUSTOM
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

  struct start_point2d{
      int x=-1;
      int y=-1;
      int gridx=-1;
      int gridy=-1;
      int layer=1;
      std::string start_scene="01";
      std::string direction="upward";

      start_point2d operator=(start_point2d& copyone){
          x=copyone.x;
          y=copyone.y;
          gridx=copyone.gridx;
          gridy=copyone.gridy;
          layer=copyone.layer;
          start_scene=copyone.start_scene;
          direction=copyone.direction;
          return *this;
      }
  };

  typedef std::vector<int> ROW_IDX;

  typedef std::vector<ROW_IDX> LAYER_IDX;

}


#endif // COMMON_STRUCT_H_INCLUDED
