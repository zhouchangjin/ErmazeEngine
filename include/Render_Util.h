#ifndef RENDER_UTIL_H_INCLUDED
#define RENDER_UTIL_H_INCLUDED


#include <string>
#include "SDL_Utilities.h"
#include "Str_Utilities.h"
#include "CGameContext.h"
#include "CParticle.h"
#include "CSprite.h"
#include "C2DGameScene.h"
#include "CTileLayer.h"
#include "CAdvDialog.h"
#include "CChoiceDialog.h"
#include "CImageDB.h"
#include "CGameDatabase.h"

namespace sdlutil2{

    SDL_Renderer* GetRenderer(CGameContext* p_context);
    SDL_Window* GetWindow(CGameContext* p_context);

    void* LoadPngTexture(std::string path,CGameContext* p_context);

    void* LoadFont(std::string path);

    void RenderSceneLayer(CGameContext* p_context,C2DGameScene& scene,int layer_idx,int camerax,int cameray,ge_common_struct::ge_rect window,int scale);

    void RenderSprite(CGameContext* p_context,CSprite* sprite,int screenx,int screeny,int sprite_idx,int scale,int alpha=255);

    void MaskSprite(CGameContext* p_context,CSprite* sprite,int screenx,int screeny,int sprite_idx,int scale,ge_common_struct::ge_adv_color mask_color,ge_common_struct::ge_color ignore_color);

    void RenderGameObject(CGameContext* p_context,CSpriteGameObject* object,ge_common_struct::ge_rect window,int camerax,int cameray,int scale);

    void RenderTexture(CGameContext* p_context,int x,int y,ge_common_struct::ge_rect rect,void* texture,int scale);

    void RenderPresent(CGameContext* p_context);

    void FillRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,int g,int b,int a=255);

    void FillRoundRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int radius,ge_common_struct::ge_adv_color color);

    void DrawRoundRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int radius,ge_common_struct::ge_adv_color color);

    ge_common_struct::ge_rect LoadWindowRect(CGameContext* p_context);

    void DrawRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,int g,int b,int a);

    void FillTriangle(CGameContext* p_context,ge_common_struct::ge_triangle triangle,ge_common_struct::ge_color color);

    void RenderText(CGameContext* p_context,void* font,int x,int y,std::string textureText,ge_common_struct::ge_color text_color,int scale=1);

    void SetAlphaMode(C2DGameScene& gamescene,int alpha);

    void DrawWindow(CGameContext* p_context,CGameWindow& window);

    void DrawDialog(CGameContext* p_context,CGameDialog& dialog);

    void DrawAdvDialog(CGameContext* p_context,CAdvDialog& dialog);

    void DrawChoiceDialog(CGameContext* p_context,CChoiceDialog& dialog);

    void DrawPointDownTriangle(CGameContext* p_context,int x,int y,int height,ge_common_struct::ge_color color);

    void DrawDomNode(CGameContext* p_context,ge_common_struct::dom_node* node,CImageDB* imagedb);

    int GetMaxPageCntOfDomNode(ge_common_struct::dom_node* node,CGameDatabase* gamedb);

    void UpdateDomNode(ge_common_struct::dom_node* node,CGameDatabase* gamedb,int context_obj=-1,int page_start=0);

    void UpdateDomRect(ge_common_struct::dom_node* node,ge_common_struct::ge_rect parent_rect);

    void DrawTexture(CGameContext* p_context,ge_common_struct::dom_node* node,unsigned int pointer_pos,CTiledTexture texture,std::string name,int offsetx=0,int offsety=0,int scale=1);

    void DrawTexture2(CGameContext* p_context,ge_common_struct::ge_rect rect,CTexture texture,int scale=1);

    void DrawBorder(CGameContext* context,ge_common_struct::ge_rect rect,ge_common_struct::ge_adv_color color,ge_common_struct::ge_sides border);

    void FillRectTexture(CGameContext* p_context,ge_common_struct::ge_rect rect,CTiledTexture texture,std::string texture_name,int scale=1);

    void RenderParticles(CGameContext* p_context,std::vector<CParticle*> particles,CTiledTexture texture);

}

#endif // RENDER_UTIL_H_INCLUDED
