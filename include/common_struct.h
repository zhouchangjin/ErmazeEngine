#ifndef COMMON_STRUCT_H_INCLUDED
#define COMMON_STRUCT_H_INCLUDED

#include <vector>
#include <string>
#include <queue>

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

  enum action_type{
      NO_MOVE,
      MOVE_UP,
      MOVE_DOWN,
      MOVE_LEFT,
      MOVE_RIGHT,
  };

  struct action_log{
    std::string action;
    int move_x;
    int move_y;
    action_log operator=(action_log& copylog){
        action=copylog.action;
        move_x=copylog.move_x;
        move_y=copylog.move_y;
        return *this;
    };
    action_log(){
    };
    action_log(const action_log& other){
        action=other.action;
        move_x=other.move_x;
        move_y=other.move_y;
    };
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
