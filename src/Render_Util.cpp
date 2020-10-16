#include "Render_Util.h"
//该工具方法可以被面向对象的Service类取代

namespace sdlutil2
{

SDL_Renderer* GetRenderer(CGameContext* p_context)
{
    CSdlGameContext* context=(CSdlGameContext*)p_context;
    SDL_Renderer* renderer=context->GetRenderer();
    return renderer;
}


void* LoadPngTexture(std::string path,CGameContext* p_context)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    SDL_Texture* texture=sdlutil::LoadPngTexture(path,renderer);
    return (void*)texture;
}

void* LoadFont(std::string path)
{
    TTF_Font* font=sdlutil::LoadFont(path);
    return (void*)font;
}

SDL_Color TransformColor(ge_common_struct::ge_color color)
{
    SDL_Color rColor;
    rColor.r=color.r;
    rColor.g=color.g;
    rColor.b=color.b;
    return rColor;
}

SDL_Rect TransformRect(ge_common_struct::ge_rect rect)
{
    SDL_Rect rRect;
    rRect.x=rect.x;
    rRect.y=rect.y;
    rRect.w=rect.w;
    rRect.h=rect.h;
    return rRect;
}

void RenderTexture(CGameContext* p_context,int x,int y,ge_common_struct::ge_rect rect,void* texture,int scale)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    SDL_Rect clip=TransformRect(rect);
    SDL_Texture* sdltexture=(SDL_Texture*)texture;
    sdlutil::RenderTexture(x,y,&clip,sdltexture,renderer,scale);
}


void RenderSprite(CGameContext* p_context,CSprite* sprite,int sprite_idx,int screenx,int screeny,int scale)
{
    CSpriteSheet* spritesheet=sprite->GetSpriteSheet();
    void * texture=spritesheet->GetTexture();
    SDL_Texture* sdl_texture=(SDL_Texture* )texture;
    SDL_Renderer * renderer=GetRenderer(p_context);

    ge_common_struct::ge_rect rect=sprite->GetRectByIdx(sprite_idx);
    SDL_Rect sdlrect=TransformRect(rect);

    sdlutil::RenderTexture(screenx,screeny,&sdlrect,sdl_texture,renderer,scale);
}


void RenderPresent(CGameContext* p_context)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    SDL_RenderPresent(renderer);
}


void FillRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,int g,int b,int a)
{

    SDL_Renderer * renderer=GetRenderer(p_context);
    SDL_SetRenderDrawColor( renderer, r, g, b, a );
    SDL_Rect sdlrect=TransformRect(rect);
    SDL_RenderFillRect( renderer, &sdlrect );
}

void DrawRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,int g,int b,int a)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    SDL_Rect sdlrect=TransformRect(rect);
    SDL_SetRenderDrawColor( renderer, r, g, b, a );
    SDL_RenderDrawRect(renderer,&sdlrect);
}


void RenderText(CGameContext* p_context,void* font,int x,int y,std::string textureText,ge_common_struct::ge_color text_color,int scale)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    TTF_Font * ttlfont=(TTF_Font*)font;
    SDL_Color sdlcolor=TransformColor(text_color);
    sdlutil::RenderText(ttlfont,renderer,x,y,textureText,sdlcolor,scale);
}

ge_common_struct::ge_rect LoadWindowRect(CGameContext* p_context)
{
    ge_common_struct::ge_rect rect=p_context->GetWindowSize();
    return rect;
}

void RenderSceneLayer(CGameContext* p_context,CGameScene& scene,
                      int layer_idx,int centerx,int centery,
                      ge_common_struct::ge_rect window,int scale)
{

    int tilewidth=scene.GetTileWidth();
    int tileheight=scene.GetTileHeight();

    SDL_Texture* sdl_texture=(SDL_Texture*)scene.GetTexture();
    int width;int height;
    sdlutil::LoadTextureWidthHeight(sdl_texture,&width,&height);
    int tileset_cols=width/tilewidth;
    //int tileset_rows=height/tileheight;

    SDL_Renderer* renderer=GetRenderer(p_context);
    ge_common_struct::LAYER_IDX layer=scene.GetLayer(layer_idx);
    int start_screen_x=centerx*scale-window.w/2;
    int start_screen_y=centery*scale-window.h/2;

    int start_x=start_screen_x/2;
    int start_y=start_screen_y/2;

    int start_screen_coorx=-1*(start_x%tilewidth);
    int start_screen_coory=-1*(start_y%tileheight);

    int min_col=start_x/tilewidth;
    int min_row=start_y/tileheight;

    int max_col=(start_x+window.w/scale)/tilewidth;
    int max_row=(start_y+window.h/scale)/tileheight;



    for(int j=min_row;j<=max_row;j++){

        for(int i=min_col;i<=max_col;i++){

            if(j<0 || j>=((int)layer.size())){
                continue;
            }
            ge_common_struct::ROW_IDX row=layer[j];
            if(i<0 || i>=((int)row.size())){
                continue;
            }

            int idx=row[i]-1; //注意这里是因为原来tmx文件是从1开始的编号的

            int tileset_row_num=idx/tileset_cols;
            int tileset_col_num=idx%tileset_cols;

            ge_common_struct::ge_rect rect;
            rect.w=tilewidth;
            rect.h=tileheight;
            rect.x=tileset_col_num*tilewidth;
            rect.y=tileset_row_num*tileheight;
            SDL_Rect sdlrect=TransformRect(rect);
            int c_screen_x=scale*(start_screen_coorx+(i-min_col)*tilewidth);
            int c_screen_y=scale*(start_screen_coory+(j-min_row)*tileheight);
            sdlutil::RenderTexture(c_screen_x,c_screen_y,&sdlrect,sdl_texture,renderer,scale);



        }
    }


}

}
