#ifndef CGAMEWINDOW_H
#define CGAMEWINDOW_H

#include <common_struct.h>

class CGameWindow
{
    public:
        CGameWindow();
        virtual ~CGameWindow();
        int GetX()const{return m_window_rect.x;};
        int GetY()const{return m_window_rect.y;};
        int GetWidth()const{return m_window_rect.w;};
        int GetHeight()const{return m_window_rect.h;};
        int GetTitleX()const{return m_title_rect.x;};
        int GetTitleY()const{return m_title_rect.y;};
        int GetTitleWidth()const{return m_title_rect.w;};
        int GetTitleHeight()const{return m_title_rect.h;};
        std::string GetTitle()const{return m_title;};
        int GetBorderWidth()const{return m_border_width;};
        ge_common_struct::ge_color GetBorderColor()const
        {return m_bd_color;};
        ge_common_struct::ge_adv_color GetBackgroundColor()const
        {return m_bg_color;};
        ge_common_struct::ge_color GetFontColor()const{return m_font_color;};
        ge_common_struct::ge_rect GetWindowRect()const;
        ge_common_struct::ge_rect GetTitleRect()const;
        bool IsShow()const{return m_show;};


        void SetX(int x){m_window_rect.x=x;};
        void SetY(int y){m_window_rect.y=y;};
        void SetWidth(int width){m_window_rect.w=width;};
        void SetHeight(int height){m_window_rect.h=height;};
        void SetTitleX(int tx){m_title_rect.x=tx;};
        void SetTitleY(int ty){m_title_rect.y=ty;};
        void SetTitleWidth(int tw){m_title_rect.w=tw;};
        void SetTitleHeight(int th){m_title_rect.h=th;};
        void SetTitle(std::string title){m_title=title;};
        void SetBackGroundColor(int red,int green,int blue,int alpha);
        void SetBorderColor(int red,int green,int blue);
        void SetBorderWidth(int width){m_border_width=width;};
        void SetFontColor(int red,int green,int blue);
        void Show(){m_show=true;};
        void Hide(){m_show=false;};
    protected:

        std::string m_title;
        int m_border_width=2;
        ge_common_struct::ge_rect m_window_rect;
        ge_common_struct::ge_rect m_title_rect;
        ge_common_struct::ge_adv_color m_bg_color;
        ge_common_struct::ge_color m_bd_color;
        ge_common_struct::ge_color m_font_color;
        bool m_show=false;
    private:
};

#endif // CGAMEWINDOW_H
