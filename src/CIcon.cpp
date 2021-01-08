#include "CIcon.h"

CIcon::CIcon(CTiledIcon* icon,int idx)
{
    m_icon_sheet=icon;
    m_icon_idx=idx;
}

CIcon::~CIcon()
{
    //dtor
}
