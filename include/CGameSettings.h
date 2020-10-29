#ifndef CGAMESETTINGS_H
#define CGAMESETTINGS_H


class CGameSettings
{

    public:
        CGameSettings();
        virtual ~CGameSettings();
        int GetWindowWidth(){return m_window_width;};
        int GetWindowHeight(){return m_window_height;};
        int GetFramePerSecond(){return m_fps;};
        void SetWidthHeight(int width,int height){m_window_width=width;m_window_height=height;};
        void SetFramePerSecond(int fps){m_fps=fps;};
        int GetMinFrameTime(){return 1000/m_fps;};
    protected:

    private:
        int m_window_width=1024;
        int m_window_height=768;
        int m_fps=24;
};

#endif // CGAMESETTINGS_H
