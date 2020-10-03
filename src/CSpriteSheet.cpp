#include "CSpriteSheet.h"

CSpriteSheet::CSpriteSheet()
{
    //ctor
}

CSpriteSheet::~CSpriteSheet()
{
    //dtor
}



ge_common_struct::ge_rect CSpriteSheet::GetRectByIdx(int index){

    int row=index/m_resource_width;
    int col=index%m_resource_width;

    ge_common_struct::ge_rect rect;
    rect.x=m_sprite_width*col;
    rect.y=m_sprite_height*row;
    rect.w=m_sprite_width;
    rect.h=m_sprite_height;
    return rect;
}
