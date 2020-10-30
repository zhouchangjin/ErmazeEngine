#ifndef RENDER_UTIL_H_INCLUDED
#define RENDER_UTIL_H_INCLUDED


#include <string>
#include "SDL_Utilities.h"
#include "CSDLGameContext.h"
#include "CSprite.h"
#include "C2DGameScene.h"
#include "CTileLayer.h"

namespace sdlutil2{

    SDL_Renderer* GetRenderer(CGameContext* p_context);

    void* LoadPngTexture(std::string path,CGameContext* p_context);

    void* LoadFont(std::string path);

    void RenderSceneLayer(CGameContext* p_context,C2DGameScene& scene,int layer_idx,int camerax,int cameray,ge_common_struct::ge_rect window,int scale);

    void RenderSprite(CGameContext* p_context,CSprite* sprite,int screenx,int screeny,int sprite_idx,int scale);

    void RenderGameObject(CGameContext* p_context,CSpriteGameObject* object,ge_common_struct::ge_rect window,int camerax,int cameray,int scale);

    void RenderTexture(CGameContext* p_context,int x,int y,ge_common_struct::ge_rect rect,void* texture,int scale);

    void RenderPresent(CGameContext* p_context);

    void FillRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,int g,int b,int a=255);

    ge_common_struct::ge_rect LoadWindowRect(CGameContext* p_context);

    void DrawRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,int g,int b,int a);

    void RenderText(CGameContext* p_context,void* font,int x,int y,std::string textureText,ge_common_struct::ge_color text_color,int scale=1);

    void SetAlphaMode(C2DGameScene& gamescene,int alpha);

}

#endif // RENDER_UTIL_H_INCLUDED
