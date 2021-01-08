#ifndef CICON_H
#define CICON_H

#include<CTiledIcon.h>

class CIcon
{
    public:
        CIcon(CTiledIcon* iconsheet,int idx);
        virtual ~CIcon();
        CTiledIcon* GetTiledIcon(){return m_icon_sheet;};
        int GetIdx(){return m_icon_idx;};
    protected:
        CTiledIcon* m_icon_sheet;
        int m_icon_idx;

    private:
};

#endif // CICON_H
