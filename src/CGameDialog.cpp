#include "CGameDialog.h"

CGameDialog::CGameDialog()
{
    //ctor
}

CGameDialog::~CGameDialog()
{
    //dtor
}

int CGameDialog::GetCurrentDialogLineCharCnt(){

   int text_width=m_window_rect.w-m_border_width*2-m_text_margin*2;
   int line_char_cnt=text_width/m_font_size;
   return line_char_cnt;
}

void CGameDialog::TextUpdate(){
    m_current_pos+=m_display_speed;
}

void CGameDialog::SetText(std::string text){
    m_text=text;
    m_current_pos=m_display_speed;
}
