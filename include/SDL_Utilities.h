#ifndef SDL_UTILITIES_H_INCLUDED
#define SDL_UTILITIES_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>


namespace sdlutil{

    SDL_Texture* LoadPngTexture(std::string filePath,SDL_Renderer * renderer);

    void RenderTexture(int x,int y,SDL_Rect* clip,SDL_Texture* texture,SDL_Renderer* renderer,int scale);

    TTF_Font* LoadFont(std::string fontPath);

    void RenderText(TTF_Font* font,SDL_Renderer* renderer,int x,int y,std::string textureText,SDL_Color textColor,int scale=1);

}

#endif // SDL_UTILITIES_H_INCLUDED
