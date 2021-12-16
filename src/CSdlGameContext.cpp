#include "CSdlGameContext.h"

CSdlGameContext::CSdlGameContext()
{
    //ctor
}

CSdlGameContext::~CSdlGameContext()
{
    //dtor
}

SDL_Renderer* CSdlGameContext::GetRenderer()
{
    return m_renderer;
}

SDL_Window* CSdlGameContext::GetWindow(){
    return m_window;
}

ge_common_struct::input_event CSdlGameContext::EventCatch()
{

    SDL_Event e;
    ge_common_struct::input_event translate_event;
    while(SDL_PollEvent( &e ) != 0 )
    {
        Uint32 tick=SDL_GetTicks();
        Uint32 key_time=e.key.timestamp;
        if(tick-key_time>300)
        {
            continue;
        }
        if( e.type == SDL_QUIT )
        {
            translate_event.add_event(ge_common_struct::QUIT);
            break;
        }
        else if(e.type==SDL_KEYDOWN)
        {
            //GE_LOG(" kecode %d :repeat-%d,state-%d     \n",e.key.keysym.scancode,e.key.repeat,e.key.state);
            if(e.key.repeat==0)
            {
                switch( e.key.keysym.sym )
                {
                case SDLK_UP:
                    translate_event.add_event(ge_common_struct::KEY_UP,
                                              ge_common_struct::key_press_type
                                              ::KEY_PRESS,SDL_SCANCODE_UP);
                    break;

                case SDLK_DOWN:
                    translate_event.add_event(ge_common_struct::KEY_DOWN,
                                              ge_common_struct::key_press_type
                                              ::KEY_PRESS,SDL_SCANCODE_DOWN);
                    break;

                case SDLK_LEFT:
                    translate_event.add_event(ge_common_struct::KEY_LEFT,
                                              ge_common_struct::key_press_type
                                              ::KEY_PRESS,SDL_SCANCODE_LEFT);
                    break;

                case SDLK_RIGHT:
                    translate_event.add_event(ge_common_struct::KEY_RIGHT,
                                              ge_common_struct::key_press_type
                                              ::KEY_PRESS,SDL_SCANCODE_RIGHT);
                    break;
                case SDLK_SPACE:
                    translate_event.add_event(ge_common_struct::KEY_SPACE,
                                              ge_common_struct::key_press_type
                                              ::KEY_PRESS,SDL_SCANCODE_SPACE);
                    break;
                case SDLK_ESCAPE:
                    translate_event.add_event(ge_common_struct::KEY_ESC,
                                              ge_common_struct::key_press_type
                                              ::KEY_PRESS,SDL_SCANCODE_ESCAPE);
                    break;
                case SDLK_RETURN:
                    translate_event.add_event(ge_common_struct::KEY_ENTER,
                                              ge_common_struct::key_press_type::
                                              KEY_PRESS,SDL_SCANCODE_RETURN);
                    break;
                default:
                    if(m_key_mapping.find(e.key.keysym.sym)!=m_key_mapping.end())
                    {
                        translate_event.add_event(
                            m_key_mapping[e.key.keysym.sym],
                            ge_common_struct::key_press_type::KEY_PRESS,
                            e.key.keysym.scancode);
                    }
                    break;
                }


            }
            break;
        }
    }
    /*
    if(!translate_event.empty()){
        return translate_event;
    }*/

    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if(keystates[SDL_SCANCODE_UP])
    {
        translate_event.add_event(ge_common_struct::key_event_type::KEY_UP,
                                  ge_common_struct::key_press_type::KEY_PRESSED,
                                  SDL_SCANCODE_UP);
    }
    if(keystates[SDL_SCANCODE_DOWN])
    {
        translate_event.add_event(ge_common_struct::key_event_type::KEY_DOWN,
                                  ge_common_struct::key_press_type::KEY_PRESSED,
                                  SDL_SCANCODE_DOWN);
    }
    if(keystates[SDL_SCANCODE_LEFT])
    {
        translate_event.add_event(ge_common_struct::key_event_type::KEY_LEFT,
                                  ge_common_struct::key_press_type::KEY_PRESSED,
                                  SDL_SCANCODE_LEFT);

    }
    if(keystates[SDL_SCANCODE_RIGHT])
    {
        translate_event.add_event(ge_common_struct::key_event_type::KEY_RIGHT,
                                  ge_common_struct::key_press_type::KEY_PRESSED,
                                  SDL_SCANCODE_RIGHT);
    }
    if(keystates[SDL_SCANCODE_SPACE])
    {
        translate_event.add_event(ge_common_struct::key_event_type::KEY_SPACE,
                                  ge_common_struct::key_press_type::KEY_PRESSED,
                                  SDL_SCANCODE_SPACE);
    }
    if(keystates[SDL_SCANCODE_RETURN])
    {
        translate_event.add_event(ge_common_struct::key_event_type::KEY_ENTER,
                                  ge_common_struct::key_press_type::KEY_PRESSED,
                                  SDL_SCANCODE_RETURN);
    }
    if(keystates[SDL_SCANCODE_ESCAPE])
    {
        translate_event.add_event(ge_common_struct::key_event_type::KEY_ESC,
                                  ge_common_struct::key_press_type::KEY_PRESSED,
                                  SDL_SCANCODE_ESCAPE);
    }
    std::map<int,ge_common_struct::key_event_type>::iterator it;
    for(it=m_key_mapping.begin(); it!=m_key_mapping.end(); it++)
    {
        int keycharcode=it->first;
        SDL_Scancode scancode= SDL_GetScancodeFromKey(keycharcode);
        if(keystates[scancode])
        {
            translate_event.add_event(m_key_mapping[keycharcode],ge_common_struct::key_press_type::KEY_PRESSED,scancode);
        }
    }

    return translate_event;

}


void CSdlGameContext::Init(int window_width,int window_height)
{


    Uint32 flags=SDL_INIT_VIDEO;
    if ( SDL_Init( flags ) != 0 )
    {


    }
    else
    {
        GE_LOG("Initialize....\n");
    }
    if ( SDL_CreateWindowAndRenderer( window_width, window_height, SDL_WINDOW_SHOWN,
                                      &m_window, &m_renderer ) != 0 )
    {

    }
    else
    {
        GE_LOG("Window Create Done.\n");
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {

    }
    else
    {
        GE_LOG("Image initial Successful.\n");
    }

    if( TTF_Init() == -1 )
    {

    }
    else
    {
        std::string path="./fonts/"+m_font_name+".ttf";
        m_font=sdlutil::LoadFont(path);
        GE_LOG("Font Load Successful.\n");
    }

}

ge_common_struct::ge_rect CSdlGameContext::GetWindowSize()
{
    int height,width;
    SDL_GetWindowSize(m_window,&width,&height);

    ge_common_struct::ge_rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=width;
    rect.h=height;
    return rect;
}

void CSdlGameContext::DelayTime(uint32_t time)
{
    SDL_Delay(time);
}

unsigned int CSdlGameContext::GetTicks()
{
    return SDL_GetTicks();
}

void CSdlGameContext::ClearUp()
{

    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void CSdlGameContext::RegisterKey(char keyname,ge_common_struct::key_event_type e)
{
    int intKey=(int)keyname;
    m_key_mapping[intKey]=e;
}
