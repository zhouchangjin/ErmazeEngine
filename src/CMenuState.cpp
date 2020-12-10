#include "CMenuState.h"

CMenuState::CMenuState()
{
    //ctor
}

CMenuState::CMenuState(CGameContext * context):CGameState(context)
{

}

CMenuState::~CMenuState()
{
    //dtor
}

void CMenuState::Init()
{
    m_state_value=0;
    m_font=sdlutil2::LoadFont(m_font_path);
    m_background_texture=sdlutil2::LoadPngTexture(m_background_path,m_context);

    m_select_rect.x=370;
    m_select_rect.y=0;
    m_select_rect.w=200;
    m_select_rect.h=50;

}

void CMenuState::Cleanup()
{

}

void CMenuState::Draw()
{
    m_select_rect.y=440+m_select_idx*50;
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    sdlutil2::RenderTexture(m_context,0,0,fullWindow,m_background_texture,1);

    ge_common_struct::ge_color color;
    color.r=255;
    color.g=255;
    color.b=255;

    sdlutil2::RenderText(m_context,m_font,400,450,m_menu_1,color);
    sdlutil2::RenderText(m_context,m_font,400,500,m_menu_2,color);
    sdlutil2::RenderText(m_context,m_font,400,550,m_menu_3,color);
    sdlutil2::DrawRect(m_context,m_select_rect,255,255,0,255);
    sdlutil2::RenderPresent(m_context);
}

void CMenuState::Pause()
{

}


void CMenuState::Resume()
{
    this->m_state_value=0;
    m_menu_1="Resume Game";
}

void CMenuState::HandleEvent(ge_common_struct::input_event event)
{

    ge_common_struct::key_event key_event=event.get_top_event();
    if(key_event.press==ge_common_struct::key_press_type::KEY_PRESS)
    {
        ge_common_struct::key_event_type key_type=key_event.key;

        if(key_type==ge_common_struct::KEY_DOWN)
        {
            if(m_select_idx<2)
                m_select_idx++;
        }
        else if(key_type==ge_common_struct::KEY_UP)
        {
            if(m_select_idx>0)
                m_select_idx--;
        }
        else if(key_type==ge_common_struct::KEY_SPACE
                || key_type==ge_common_struct::KEY_ENTER)
        {

            if(m_select_idx==0)
            {
                //switch state to
                m_state_value=2;
            }
            else if(m_select_idx==2)
            {
                m_state_value=-1;
            }

        }

    }



}


void CMenuState::Update()
{

}

int CMenuState::GetStateValue()
{
    return m_state_value;
}
