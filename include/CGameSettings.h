#ifndef CGAMESETTINGS_H
#define CGAMESETTINGS_H


class CGameSettings
{

    public:
        CGameSettings();
        virtual ~CGameSettings();
        int GetWindowWidth(){return m_window_width;};
        int GetWindowHeight(){return m_window_height;};
        void SetWidthHeight(int width,int height){m_window_width=width;m_window_height=height;};
    protected:

    private:
        int m_window_width=1024;
        int m_window_height=768;
};

#endif // CGAMESETTINGS_H
