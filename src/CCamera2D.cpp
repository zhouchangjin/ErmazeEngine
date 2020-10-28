#include "CCamera2D.h"

CCamera2D::CCamera2D(){
    m_camera_x=0;
    m_camera_y=0;
}

CCamera2D::CCamera2D(int x,int y)
{
    m_camera_x=x;
    m_camera_y=y;
}

CCamera2D::~CCamera2D()
{
    //dtor
}
