#include "CGameUISystem.h"
CGameUISystem::CGameUISystem(){
}

CGameUISystem::CGameUISystem(CGameContext* context)
{
    m_context=context;
}

CGameUISystem::~CGameUISystem()
{
    //dtor
}


void CGameUISystem::InitDialog(){
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    m_dialog.SetX(fullWindow.x);
    m_dialog.SetY(fullWindow.h*2/3);
    m_dialog.SetWidth(fullWindow.w);
    m_dialog.SetHeight(fullWindow.h/3);
    m_dialog.SetBackGroundColor(0,0,0,200);
    m_dialog.SetTitleHeight(50);
    m_dialog.SetTitleWidth(200);
    m_dialog.SetTitleX(fullWindow.x);
    m_dialog.SetTitleY(m_dialog.GetY()-m_dialog.GetTitleHeight()
                       +m_dialog.GetBorderWidth());
    m_dialog.SetFontColor(255,255,255);
    m_dialog.InitResponseByDialog();
}

void CGameUISystem::ProcessInput(CInputEvent event){

}
