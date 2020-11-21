#include "CChoiceDialog.h"

CChoiceDialog::CChoiceDialog()
{
    //ctor
}

CChoiceDialog::~CChoiceDialog()
{
    //dtor
}

int CChoiceDialog::GetChoiceTextXStart(){
    ge_common_struct::ge_rect rect=GetWindowRect();
    int xstart=rect.x+m_left_margin+m_indicator_size;
    return xstart;
}

int CChoiceDialog::GetChoiceTextYStart(){
    ge_common_struct::ge_rect rect=GetWindowRect();
    int ystart=rect.y+m_top_margin+GetBorderWidth();
    return ystart;

}

ge_common_struct::ge_triangle CChoiceDialog::GetIndicator(){

    int x1=GetChoiceTextXStart()-m_indicator_size;
    int y1=GetChoiceTextYStart()+m_current_selection*m_lineheight;
    int x2=x1;
    int y2=y1+m_indicator_size;
    int x3=x1+m_indicator_size;
    int y3=y1+m_indicator_size/2;
    ge_common_struct::ge_triangle triangle(x1,y1,x2,y2,x3,y3);
    return triangle;
}
