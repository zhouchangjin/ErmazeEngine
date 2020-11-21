#ifndef CSDLGAMECONTEXT_H
#define CSDLGAMECONTEXT_H


#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_Utilities.h>
#include <Render_Util.h>
#include <CGameContext.h>


class CSdlGameContext : public CGameContext
{
    public:
        CSdlGameContext();
        virtual ~CSdlGameContext();
        void Init(int window_width,int window_height);
        void ClearUp();
        ge_common_struct::input_event EventCatch();
        SDL_Renderer* GetRenderer();
        ge_common_struct::ge_rect GetWindowSize();
        unsigned int GetTicks();
        void DelayTime(uint32_t time);
        void* GetFont()const{return (void*)m_font;};

    protected:

    private:
        SDL_Renderer * m_renderer;
        SDL_Window * m_window;
        TTF_Font *m_font = nullptr;
        std::string m_font_name="simhei";
};

#endif // CSDLGAMECONTEXT_H
