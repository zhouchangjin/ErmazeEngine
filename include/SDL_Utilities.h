#ifndef SDL_UTILITIES_H_INCLUDED
#define SDL_UTILITIES_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <string>


namespace sdlutil{

    void LoadTextureWidthHeight(SDL_Texture* texture,int* w,int* h);

    SDL_Texture* LoadPngTexture(std::string filePath,SDL_Renderer * renderer);

    SDL_Surface* LoadSurface(std::string filePath);

    SDL_Surface* LoadFormatSurface(std::string path,SDL_Window* window);

    void RenderTexture(int x,int y,SDL_Rect* clip,SDL_Texture* texture,SDL_Renderer* renderer,int scale);

    TTF_Font* LoadFont(std::string fontPath);

    void RenderText(TTF_Font* font,SDL_Renderer* renderer,int x,int y,std::string textureText,SDL_Color textColor,int scale=1);

    void SetTextureAlpha(SDL_Texture* texture,int alpha);

    void EnableAndSetTextureAlpha(SDL_Texture* texture,int alpha);

    void SetTextureBlendMode(SDL_Texture* texture,SDL_BlendMode blending);

    void FillTrangle(SDL_Renderer* renderer,int x,int y,int x1,int y1,int x2,int y2,int r,int g,int b);

    void FillRoundRect(SDL_Renderer* renderer,int x,int y,int w,int h,int radius,int r,int g,int b,int a);

    void DrawRoundRect(SDL_Renderer* renderer,int x,int y,int w,int h,int radius,int r,int g,int b,int a);

    void FillRectWithTexture(SDL_Renderer* renderer,int x,int y,int w,int h,SDL_Texture* texture,int sx,int sy,int sw,int sh,int scale=1);


}

#endif // SDL_UTILITIES_H_INCLUDED
