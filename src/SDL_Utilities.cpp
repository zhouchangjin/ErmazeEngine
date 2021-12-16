#include "SDL_Utilities.h"

namespace sdlutil
{

void LoadTextureWidthHeight(SDL_Texture* texture,int* w,int* h)
{
    SDL_QueryTexture(texture,NULL,NULL,w,h);
}

SDL_Surface* LoadSurface(std::string filePath){

    SDL_Surface* loadedSurface = IMG_Load( filePath.c_str() );
    return loadedSurface;
}

SDL_Surface* LoadFormatSurface(std::string path,SDL_Window* window){
     SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
     if(loadedSurface==NULL){ //这里不用nullptr
        return nullptr;
     }else{
         SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface
                                    , SDL_GetWindowPixelFormat( window ), 0 );

         SDL_FreeSurface( loadedSurface );
         return formattedSurface;
     }
}


SDL_Texture* LoadPngTexture(std::string path,SDL_Renderer * renderer)
{

    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {

    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
        {

        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;


}

void RenderTexture(int x,int y,SDL_Rect* clip,SDL_Texture* texture,SDL_Renderer* renderer,int scale)
{


    SDL_Rect renderQuad;
    renderQuad.x=x;
    renderQuad.y=y;
    renderQuad.w=clip->w*scale;
    renderQuad.h=clip->h*scale;

    SDL_RenderCopy( renderer, texture, clip, &renderQuad );

}

TTF_Font* LoadFont(std::string fontPath)
{
    TTF_Font* font = TTF_OpenFont( fontPath.c_str(), 24 );
    return font;
}

void RenderText(TTF_Font* font,SDL_Renderer* renderer,int x,int y,std::string textureText,SDL_Color textColor,int scale)
{

    SDL_Surface* textSurface = TTF_RenderUTF8_Solid( font, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {

    }
    else
    {
        SDL_Texture* tTexture;
        tTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( tTexture == NULL )
        {

        }
        else
        {
            SDL_Rect rect;
            rect.h=textSurface->h;
            rect.w=textSurface->w;
            rect.x=0;
            rect.y=0;

            RenderTexture(x,y,&rect,tTexture,renderer,scale);
            SDL_DestroyTexture( tTexture );

        }
        SDL_FreeSurface( textSurface );
    }


}

void SetTextureBlendMode(SDL_Texture* texture,SDL_BlendMode blending){
     SDL_SetTextureBlendMode( texture, blending );
}

void EnableAndSetTextureAlpha(SDL_Texture* texture,int alpha){
    SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
    SetTextureAlpha(texture,alpha);
}

void SetTextureAlpha(SDL_Texture* texture,int alpha)
{
    SDL_SetTextureAlphaMod(texture,alpha);
}

void FillTrangle(SDL_Renderer* renderer,int x,int y,
                 int x1,int y1,int x2,int y2,int r,int g,int b)
{
    filledTrigonRGBA(renderer,x,y,x1,y1,x2,y2,r,g,b,255);
}

void FillRoundRect(SDL_Renderer* renderer,
                   int x,int y,
                   int w,int h,
                   int radius,
                   int r,int g,int b,int a)
{
    roundedBoxRGBA(renderer,x,y,x+w,y+h,radius,r,g,b,a);
}

void DrawRoundRect(SDL_Renderer* renderer,
                   int x,int y,
                   int w,int h,
                   int radius,
                   int r,int g,int b,int a)
{
    roundedRectangleRGBA(renderer,x,y,x+w,y+h,radius,r,g,b,a);

}

void FillRectWithTexture(SDL_Renderer* renderer,int x,int y,int w,int h,
                         SDL_Texture* texture,int sx,int sy,int sw,int sh,
                         int scale)
{

    int cnt_x=w/(sw*scale);
    int cnt_y=h/(sh*scale);
    int rest_w=w-sw*scale*cnt_x;
    int rest_h=h-sh*scale*cnt_y;
    for(int i=0;i<=cnt_y;i++){
        for(int j=0;j<=cnt_x;j++){

            int draw_x=x+j*sw*scale;
            int draw_y=y+i*sh*scale;
            int draw_w=sw*scale;
            int draw_h=sh*scale;
            if(i==cnt_y){
                draw_h=rest_h;
            }
            if(j==cnt_x){
                draw_w=rest_w;
            }
            if(draw_h!=0 && draw_w!=0){
                SDL_Rect clip;
                clip.x=sx;
                clip.y=sy;
                clip.w=draw_w/scale;
                clip.h=draw_h/scale;
                RenderTexture(draw_x,draw_y,&clip,texture,renderer,scale);
            }

        }
    }
}


}
