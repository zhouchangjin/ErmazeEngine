#ifndef CGAMEDIALOG_H
#define CGAMEDIALOG_H

#include <CGameWindow.h>

class CGameDialog: public CGameWindow
{
    public:
        CGameDialog();
        virtual ~CGameDialog();
        void SetText(std::string text);
        std::string GetText()const{return m_text;};
        int GetDisplaySpeed()const{return m_display_speed;};
        void SetDisplaySpeed(int spd){m_display_speed=spd;};
        int GetCurrentDialogLineCharCnt();
        int GetCurrentPos()const{return m_current_pos;};
        int GetTextMargin()const{return m_text_margin;};
        int GetFontSize()const{return m_font_size;};
        void TextUpdate();
    protected:
        std::string m_text;
        int m_display_speed=100;
        int m_current_pos=1;
        int m_text_margin=20;
        int m_font_size=24;
    private:
};

#endif // CGAMEDIALOG_H
