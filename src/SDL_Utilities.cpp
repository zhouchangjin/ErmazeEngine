#include "SDL_Utilities.h"

namespace sdlutil
{

void LoadTextureWidthHeight(SDL_Texture* texture,int* w,int* h)
{
    SDL_QueryTexture(texture,NULL,NULL,w,h);
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


}
