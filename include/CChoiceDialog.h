#ifndef CCHOICEDIALOG_H
#define CCHOICEDIALOG_H

#include <CGameWindow.h>


class CChoiceDialog : public CGameWindow
{
    public:
        CChoiceDialog();
        virtual ~CChoiceDialog();

        int GetTopMargin()const{return m_top_margin;};
        int GetBtnMargin()const{return m_btm_margin;};
        int GetLeftMargin()const{return m_left_margin;};
        int GetLineHeight()const{return m_lineheight;};
        size_t GetSelection()const{return m_current_selection;};

        void ClearChoice(){m_current_selection=0;m_choices.clear();};
        int GetChoiceCnt(){return m_choices.size();};
        std::string GetChoice(int index){return m_choices[index];};
        void AddChoice(std::string choice){m_choices.push_back(choice);};
        void ChoiceUp();
        void ChoiceDown();
        ge_common_struct::ge_triangle GetIndicator();
        int GetChoiceTextYStart();
        int GetChoiceTextXStart();

    protected:
        size_t m_current_selection=0;
        std::vector<std::string> m_choices;
        int m_left_margin=10;
        int m_top_margin=5;
        int m_btm_margin=10;
        int m_lineheight=30;
        int m_indicator_size=16;

    private:
};

#endif // CCHOICEDIALOG_H
