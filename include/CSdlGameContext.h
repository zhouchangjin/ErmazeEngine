#ifndef CSDLGAMECONTEXT_H
#define CSDLGAMECONTEXT_H


#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <CGameContext.h>


class CSdlGameContext : public CGameContext
{
    public:
        CSdlGameContext();
        virtual ~CSdlGameContext();
        void Init(int window_width,int window_height);
        void ClearUp();
        ge_common_struct::game_event EventCatch();
        SDL_Renderer* GetRenderer();
        ge_common_struct::ge_rect GetWindowSize();
        unsigned int GetTicks();
        void DelayTime(uint32_t time);


    protected:

    private:
        SDL_Renderer * m_renderer;
        SDL_Window * m_window;
        TTF_Font *m_font = NULL;
};

#endif // CSDLGAMECONTEXT_H
