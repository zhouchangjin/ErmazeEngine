#include "CGameWindow.h"

CGameWindow::CGameWindow()
{
    m_bd_color.r=255;
    m_bd_color.g=255;
    m_bd_color.b=255;
}

CGameWindow::~CGameWindow()
{
    //dtor
}

void CGameWindow::SetBackGroundColor(int red,int green,int blue,int alpha){
    m_bg_color.r=red;
    m_bg_color.g=green;
    m_bg_color.b=blue;
    m_bg_color.a=alpha;
}

ge_common_struct::ge_rect CGameWindow::GetWindowRect()const{
    return m_window_rect;
}

ge_common_struct::ge_rect CGameWindow::GetTitleRect()const{
    return m_title_rect;
}
void CGameWindow::SetBorderColor(int red,int green,int blue){
    m_bd_color.r=red;
    m_bd_color.g=green;
    m_bd_color.b=blue;
}

void CGameWindow::SetFontColor(int red,int green,int blue){
    m_font_color.r=red;
    m_font_color.g=green;
    m_font_color.b=blue;
}
