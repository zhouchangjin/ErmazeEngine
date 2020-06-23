#ifndef COMMON_STRUCT_H_INCLUDED
#define COMMON_STRUCT_H_INCLUDED

namespace ge_common_struct{

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

}


#endif // COMMON_STRUCT_H_INCLUDED
