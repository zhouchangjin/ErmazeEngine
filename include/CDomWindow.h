#ifndef CDOMWINDOW_H
#define CDOMWINDOW_H

#include <common_struct.h>

class CDomWindow
{
    public:
        CDomWindow();
        virtual ~CDomWindow();

        ge_common_struct::dom_node GetDomNode(){return m_node;};

    protected:
        ge_common_struct::dom_node m_node;
        int m_indicator_row=0;
        int m_indicator_col=0;
        int m_row_cnt;
        int m_col_cnt;
    private:
};

#endif // CDOMWINDOW_H
