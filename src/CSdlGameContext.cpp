#include "CSdlGameContext.h"

CSdlGameContext::CSdlGameContext()
{
    //ctor
}

CSdlGameContext::~CSdlGameContext()
{
    //dtor
}

SDL_Renderer* CSdlGameContext::GetRenderer(){
    return m_renderer;
}

ge_common_struct::game_event CSdlGameContext::EventCatch(){

    SDL_Event e;
    ge_common_struct::game_event translate_event;
    while(SDL_PollEvent( &e ) != 0 ){

             if( e.type == SDL_QUIT )
            {
                translate_event=ge_common_struct::QUIT;
                break;

            }else if(e.type == SDL_KEYDOWN){

                switch( e.key.keysym.sym )
                {
                case SDLK_UP:
                    translate_event=ge_common_struct::KEY_UP;
                    break;

                case SDLK_DOWN:
                    translate_event=ge_common_struct::KEY_DOWN;
                    break;

                case SDLK_LEFT:
                    translate_event=ge_common_struct::KEY_LEFT;
                    break;

                case SDLK_RIGHT:
                    translate_event=ge_common_struct::KEY_RIGHT;
                    break;
                case SDLK_SPACE:
                    translate_event=ge_common_struct::KEY_SPACE;
                    break;
                case SDLK_ESCAPE:
                    translate_event=ge_common_struct::KEY_ESC;
                    break;
                case SDLK_KP_ENTER:
                    translate_event=ge_common_struct::KEY_ENTER;
                    break;
                case SDLK_RETURN:
                    translate_event=ge_common_struct::KEY_ENTER;
                    break;
                default:
                    translate_event=ge_common_struct::NO_EVENT;
                    break;
                }
                break;
            }else{
                translate_event=ge_common_struct::NO_EVENT;
                break;
            }
    }

    return translate_event;

}


void CSdlGameContext::Init(int window_width,int window_height){


    Uint32 flags=SDL_INIT_VIDEO;
    if ( SDL_Init( flags ) != 0 )
    {


    }
    if ( SDL_CreateWindowAndRenderer( window_width, window_height, SDL_WINDOW_SHOWN,
                                      &m_window, &m_renderer ) != 0 )
    {
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {

    }

    if( TTF_Init() == -1 )
    {

    }

}

ge_common_struct::ge_rect CSdlGameContext::GetWindowSize(){
    int height,width;
    SDL_GetWindowSize(m_window,&width,&height);

    ge_common_struct::ge_rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=width;
    rect.h=height;
    return rect;
}

void CSdlGameContext::DelayTime(uint32_t time){
    SDL_Delay(time);
}

unsigned int CSdlGameContext::GetTicks(){
    return SDL_GetTicks();
}

void CSdlGameContext::ClearUp(){

    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
