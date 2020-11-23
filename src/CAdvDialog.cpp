#include "CAdvDialog.h"

CAdvDialog::CAdvDialog()
{
    //ctor
}

CAdvDialog::~CAdvDialog()
{
    //dtor
}

void CAdvDialog::SetResponseBackgroundColor(
    ge_common_struct::ge_adv_color color)
{
    m_choice_dialog.SetBackGroundColor(color.r,color.g,color.b,color.a);
}


void CAdvDialog::SetResponseBorderColor(ge_common_struct::ge_color color)
{

    m_choice_dialog.SetBorderColor(color.r,color.g,color.b);
}

void CAdvDialog::SetResponseWindowRect(ge_common_struct::ge_rect windowrect)
{
    m_choice_dialog.SetX(windowrect.x);
    m_choice_dialog.SetY(windowrect.y);
    m_choice_dialog.SetWidth(windowrect.w);
    m_choice_dialog.SetHeight(windowrect.h);
}

void CAdvDialog::InitResponseByDialog()
{
    int cnt=m_choice_dialog.GetChoiceCnt();
    int lineheight=m_choice_dialog.GetLineHeight();
    int topmargin=m_choice_dialog.GetTopMargin();
    int btnmargin=m_choice_dialog.GetBtnMargin();

    ge_common_struct::ge_color color=GetBorderColor();
    ge_common_struct::ge_adv_color bcolor=GetBackgroundColor();
    ge_common_struct::ge_rect rect=GetWindowRect();
    int inffered_width=200;
    rect.x+=rect.x+rect.w-inffered_width;
    rect.y-=cnt*lineheight
            +GetBorderWidth()+topmargin+btnmargin;
    rect.h=cnt*lineheight
           +GetBorderWidth()+topmargin;
    rect.w=inffered_width;
    SetResponseBorderColor(color);
    SetResponseBackgroundColor(bcolor);
    m_choice_dialog.SetX(rect.x);
    m_choice_dialog.SetY(rect.y);
    m_choice_dialog.SetWidth(rect.w);
    m_choice_dialog.SetHeight(rect.h);
    ge_common_struct::ge_color fcolor=GetFontColor();
    m_choice_dialog.SetFontColor(fcolor.r,fcolor.g,fcolor.b);
}

void CAdvDialog::initChoice(std::vector<std::string> choices){
    m_choice_dialog.ClearChoice();
    for(size_t i=0;i<choices.size();i++){
        m_choice_dialog.AddChoice(choices[i]);
    }
}




