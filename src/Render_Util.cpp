#include "Render_Util.h"
//该工具方法可以被面向对象的Service类取代

namespace sdlutil2{

    SDL_Renderer* GetRenderer(CGameContext* p_context){
         CSdlGameContext* context=(CSdlGameContext*)p_context;
         SDL_Renderer* renderer=context->GetRenderer();
         return renderer;
    }


    void* LoadPngTexture(std::string path,CGameContext* p_context){
         SDL_Renderer * renderer=GetRenderer(p_context);
         SDL_Texture* texture=sdlutil::LoadPngTexture(path,renderer);
         return (void*)texture;
    }

    void* LoadFont(std::string path){
        TTF_Font* font=sdlutil::LoadFont(path);
        return (void*)font;
    }

    SDL_Color TransformColor(ge_common_struct::ge_color color){
        SDL_Color rColor;
        rColor.r=color.r;
        rColor.g=color.g;
        rColor.b=color.b;
        return rColor;
    }

    SDL_Rect TransformRect(ge_common_struct::ge_rect rect){
        SDL_Rect rRect;
        rRect.x=rect.x;
        rRect.y=rect.y;
        rRect.w=rect.w;
        rRect.h=rect.h;
        return rRect;
    }

    void RenderTexture(CGameContext* p_context,int x,int y,ge_common_struct::ge_rect rect,void* texture,int scale){
        SDL_Renderer * renderer=GetRenderer(p_context);
        SDL_Rect clip=TransformRect(rect);
        SDL_Texture* sdltexture=(SDL_Texture*)texture;
        sdlutil::RenderTexture(x,y,&clip,sdltexture,renderer,scale);
    }


    void RenderSprite(CGameContext* p_context,CSprite* sprite,int sprite_idx,int screenx,int screeny,int scale){
        CSpriteSheet* spritesheet=sprite->GetSpriteSheet();
        void * texture=spritesheet->GetTexture();
        SDL_Texture* sdl_texture=(SDL_Texture* )texture;
        SDL_Renderer * renderer=GetRenderer(p_context);

        ge_common_struct::ge_rect rect=sprite->GetRectByIdx(sprite_idx);
        SDL_Rect sdlrect=TransformRect(rect);

        sdlutil::RenderTexture(screenx,screeny,&sdlrect,sdl_texture,renderer,scale);
    }


     void RenderPresent(CGameContext* p_context){
         SDL_Renderer * renderer=GetRenderer(p_context);
         SDL_RenderPresent(renderer);
     }


     void FillRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,int g,int b,int a){

          SDL_Renderer * renderer=GetRenderer(p_context);
          SDL_SetRenderDrawColor( renderer, r, g, b, a );
          SDL_Rect sdlrect=TransformRect(rect);
          SDL_RenderFillRect( renderer, &sdlrect );
     }

     void DrawRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,int g,int b,int a){
          SDL_Renderer * renderer=GetRenderer(p_context);
          SDL_Rect sdlrect=TransformRect(rect);
          SDL_SetRenderDrawColor( renderer, r, g, b, a );
          SDL_RenderDrawRect(renderer,&sdlrect);
     }


    void RenderText(CGameContext* p_context,void* font,int x,int y,std::string textureText,ge_common_struct::ge_color text_color,int scale){
        SDL_Renderer * renderer=GetRenderer(p_context);
        TTF_Font * ttlfont=(TTF_Font*)font;
        SDL_Color sdlcolor=TransformColor(text_color);
        sdlutil::RenderText(ttlfont,renderer,x,y,textureText,sdlcolor,scale);
    }

     ge_common_struct::ge_rect LoadWindowRect(CGameContext* p_context){
            ge_common_struct::ge_rect rect=p_context->GetWindowSize();
            return rect;
     }

}
