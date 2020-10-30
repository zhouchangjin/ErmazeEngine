#include "CTransferArea.h"

CTransferArea::CTransferArea()
{
    //ctor
}

CTransferArea::CTransferArea(const CTransferArea& copyarea){
    m_x=copyarea.GetX();
    m_y=copyarea.GetY();
    m_direction=copyarea.GetDirection();
    m_map_layer=copyarea.GetMapLayer();
    m_map_x=copyarea.GetMapX();
    m_map_y=copyarea.GetMapY();
    m_width=copyarea.GetWidth();
    m_height=copyarea.GetHeight();
    m_scene=copyarea.GetScene();
}

CTransferArea::~CTransferArea()
{
    //dtor
}
