#include "Render_Util.h"
//该工具方法可以被面向对象的Service类取代
#include "CSdlGameContext.h"
namespace sdlutil2
{

ge_common_struct::ge_point CalcTitlePos(ge_common_struct::ge_rect r,int charcnt
                                        ,int font_size=24)
{
    ge_common_struct::ge_point p;
    if(charcnt*font_size>r.w)
    {
        p.x=r.x+font_size/2;
        p.y=r.y+r.h-font_size;
    }
    else
    {
        int marginx= (r.w-charcnt*font_size)/2;
        int marginy= (r.h-font_size)/2;
        p.x=r.x+marginx;
        p.y=r.y+marginy;
    }
    return p;
}


ge_common_struct::ge_rect GetBorderLeft(ge_common_struct::ge_rect rect,
                                        int line_width)
{
    ge_common_struct::ge_rect border_left;
    border_left.x=rect.x;
    border_left.y=rect.y;
    border_left.w=line_width;
    border_left.h=rect.h;
    return border_left;
}

ge_common_struct::ge_rect GetBorderRight(ge_common_struct::ge_rect rect,
        int line_width)
{
    ge_common_struct::ge_rect border_right;
    border_right.x=rect.x+rect.w-line_width;
    border_right.y=rect.y;
    border_right.w=line_width;
    border_right.h=rect.h;
    return border_right;
}


ge_common_struct::ge_rect GetBorderBottom(ge_common_struct::ge_rect rect,
        int line_width)
{
    ge_common_struct::ge_rect bottom_line;
    bottom_line.x=rect.x;
    bottom_line.y=rect.y+rect.h-line_width;
    bottom_line.w=rect.w;
    bottom_line.h=line_width;
    return bottom_line;
}

ge_common_struct::ge_rect GetBorderTop(ge_common_struct::ge_rect rect,
                                       int line_width)
{

    ge_common_struct::ge_rect top_line;
    top_line.x=rect.x;
    top_line.y=rect.y;
    top_line.w=rect.w;
    top_line.h=line_width;
    return top_line;
}


void RenderBorder(CGameContext* context,ge_common_struct::ge_rect window_rect
                  ,int line_width,ge_common_struct::ge_color color)
{
    int red=color.r;
    int blue=color.b;
    int green=color.g;
    ge_common_struct::ge_rect border_top=GetBorderTop(window_rect,line_width);
    FillRect(context,border_top,red,green,blue,255);
    ge_common_struct::ge_rect border_bottom=GetBorderBottom(window_rect,line_width);
    FillRect(context,border_bottom,red,green,blue,255);
    ge_common_struct::ge_rect border_left=GetBorderLeft(window_rect,line_width);
    FillRect(context,border_left,red,green,blue,255);
    ge_common_struct::ge_rect border_right=GetBorderRight(window_rect,line_width);
    FillRect(context,border_right,red,green,blue,255);

}


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

void RenderTexture(CGameContext* p_context,int x,int y,
                   ge_common_struct::ge_rect rect,void* texture,int scale)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    SDL_Rect clip=TransformRect(rect);
    SDL_Texture* sdltexture=(SDL_Texture*)texture;
    sdlutil::RenderTexture(x,y,&clip,sdltexture,renderer,scale);
}

void RenderGameObject(CGameContext* p_context,CSpriteGameObject* object
                      ,ge_common_struct::ge_rect window,
                      int camera_x,int camera_y,int scale)
{
    CSprite* sprite=object->GetSprite();
    int x=object->GetX();
    int y=object->GetY();
    int centerx=window.x+window.w/2;
    int centery=window.y+window.h/2;
    int screenx=centerx+(x-camera_x)*scale;
    int screeny=centery+(y-camera_y)*scale;
    int frameidx=object->GetFrameIdx();
    RenderSprite(p_context,sprite,screenx,screeny,frameidx,scale);

}

void SetAlphaMode(C2DGameScene& scene,int alpha)
{
    SDL_Texture* sdl_texture=(SDL_Texture*)scene.GetTexture();
    sdlutil::SetTextureAlpha(sdl_texture,alpha);
}


void RenderSprite(CGameContext* p_context,CSprite* sprite,int screenx,
                  int screeny,int sprite_idx,int scale)
{
    CSpriteSheet* spritesheet=sprite->GetSpriteSheet();
    void * texture=spritesheet->GetTexture();
    if(!texture)
    {
        //load texture
        std::string path=spritesheet->GetSpritePath();
        spritesheet->SetTexture(LoadPngTexture(path,p_context));
        texture=spritesheet->GetTexture();
    }
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


void FillRect(CGameContext* p_context,ge_common_struct::ge_rect rect,int r,
              int g,int b,int a)
{

    SDL_Renderer * renderer=GetRenderer(p_context);
    SDL_SetRenderDrawColor( renderer, r, g, b, a );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Rect sdlrect=TransformRect(rect);
    SDL_RenderFillRect( renderer, &sdlrect );
}

/**
difference between draw and fill is that draw method is
painting the border of the rectangle.
**/
void DrawRect(CGameContext* p_context,ge_common_struct::ge_rect rect,
              int r,int g,int b,int a)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    SDL_Rect sdlrect=TransformRect(rect);
    SDL_SetRenderDrawColor( renderer, r, g, b, a );
    SDL_RenderDrawRect(renderer,&sdlrect);
}

void FillTriangle(CGameContext* p_context,
                  ge_common_struct::ge_triangle triangle,
                  ge_common_struct::ge_color color)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    sdlutil::DrawTrangle(renderer,triangle.p0.x,triangle.p0.y,
                         triangle.p1.x,triangle.p1.y,
                         triangle.p2.x,triangle.p2.y,color.r,color.g,color.b);
}


void RenderText(CGameContext* p_context,void* font,int x,int y,
                std::string textureText,ge_common_struct::ge_color text_color,
                int scale)
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


void DrawDialog(CGameContext* p_context,CGameDialog& dialog)
{
    DrawWindow(p_context,dialog);
    int line=dialog.GetCurrentDialogLineCharCnt();
    std::string text=dialog.GetText();
    int pos=dialog.GetCurrentPos();
    std::vector<std::string> lines=ge_str_utilities::
                                   SplitByUTF8CharPos(text,line,pos);
    for(size_t i=0; i<lines.size(); i++)
    {
        int rh=i*dialog.GetFontSize();
        int x=dialog.GetX()+dialog.GetBorderWidth()+dialog.GetTextMargin();
        int y=dialog.GetY()+dialog.GetBorderWidth()+dialog.GetTextMargin()+rh;
        RenderText(p_context,p_context->GetFont(),
                   x,y,lines[i],dialog.GetFontColor());
    }
    int tx=dialog.GetIndicatorX();
    int ty=dialog.GetIndicatorY();
    if(dialog.GetShowIndicator())
    {
        DrawPointDownTriangle(p_context,tx,ty,dialog.GetHeight()/20,dialog.GetBorderColor());
    }

}

void DrawChoiceDialog(CGameContext* p_context,CChoiceDialog& dialog){
    DrawWindow(p_context,dialog);
    ge_common_struct::ge_triangle triangle=dialog.GetIndicator();
    sdlutil2::FillTriangle(p_context,triangle,dialog.GetFontColor());
    for(int i=0; i<dialog.GetChoiceCnt(); i++)
        {
            int x=dialog.GetChoiceTextXStart();
            int y=dialog.GetChoiceTextYStart()
                  +i*dialog.GetLineHeight();
            sdlutil2::RenderText(p_context,p_context->GetFont(),x,y,
                                 dialog.GetChoice(i),dialog.GetFontColor());
        }
}

void DrawAdvDialog(CGameContext* p_context,CAdvDialog& dialog)
{
    DrawDialog(p_context,dialog);
    if(dialog.IsChoiceActive())
    {
        CChoiceDialog choice_dialog=dialog.GetChoiceDialog();
        DrawChoiceDialog(p_context,choice_dialog);
    }
}

void DrawWindow(CGameContext* p_context,CGameWindow& window)
{
    std::string title=window.GetTitle();
    int length=ge_str_utilities::utf8_strlen(title);
    bool showTitle=true;
    if(length==0)
    {
        showTitle=false;
    }
    ge_common_struct::ge_rect window_rect=window.GetWindowRect();
    int r=window.GetBackgroundColor().r;
    int g=window.GetBackgroundColor().g;
    int b=window.GetBackgroundColor().b;
    int a=window.GetBackgroundColor().a;
    FillRect(p_context,window_rect,r,g,b,a);


    int b_red=window.GetBorderColor().r;
    int b_green=window.GetBorderColor().g;
    int b_blue=window.GetBorderColor().b;

    ge_common_struct::ge_color color;
    color.r=b_red;
    color.g=b_green;
    color.b=b_blue;
    RenderBorder(p_context,window_rect,window.GetBorderWidth(),color);

    if(showTitle)
    {
        ge_common_struct::ge_rect title_rect=window.GetTitleRect();
        FillRect(p_context,title_rect,r,g,b,a);
        RenderBorder(p_context,title_rect,window.GetBorderWidth(),color);
        ge_common_struct::ge_point p=CalcTitlePos(title_rect,length);
        RenderText(p_context,p_context->GetFont(),p.x
                   ,p.y,title
                   ,window.GetFontColor());
    }

}

void RenderSceneLayer(CGameContext* p_context,C2DGameScene& scene,
                      int layer_idx,int camera_x,int camera_y,
                      ge_common_struct::ge_rect window,int scale)
{

    int tilewidth=scene.GetTileWidth();
    int tileheight=scene.GetTileHeight();

    SDL_Texture* sdl_texture=(SDL_Texture*)scene.GetTexture();
    int width;
    int height;
    sdlutil::LoadTextureWidthHeight(sdl_texture,&width,&height);
    int tileset_cols=width/tilewidth;
    //int tileset_rows=height/tileheight;

    SDL_Renderer* renderer=GetRenderer(p_context);
    CLayer* tlayer=scene.GetLayer(layer_idx);
    if(tlayer->GetLayerType()==ge_common_struct::layer_type::TILED_LAYER)
    {

        CTileLayer* tilelayer=(CTileLayer*)tlayer;
        ge_common_struct::LAYER_IDX layer=tilelayer->GetTiles();

        int start_screen_x=camera_x*scale-window.w/2;
        int start_screen_y=camera_y*scale-window.h/2;

        int start_x=start_screen_x/scale;
        int start_y=start_screen_y/scale;

        int start_screen_coorx=-1*(start_x%tilewidth);
        int start_screen_coory=-1*(start_y%tileheight);

        int min_col=start_x/tilewidth;
        int min_row=start_y/tileheight;

        int max_col=(start_x+window.w/scale)/tilewidth;
        int max_row=(start_y+window.h/scale)/tileheight;



        for(int j=min_row; j<=max_row; j++)
        {

            for(int i=min_col; i<=max_col; i++)
            {

                if(j<0 || j>=((int)layer.size()))
                {
                    continue;
                }
                ge_common_struct::ROW_IDX row=layer[j];
                if(i<0 || i>=((int)row.size()))
                {
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
                sdlutil::RenderTexture(c_screen_x,c_screen_y,&sdlrect,
                                       sdl_texture,renderer,scale);



            }
        }


    }


}

void DrawPointDownTriangle(CGameContext* p_context,int x,int y,int height,
                           ge_common_struct::ge_color color)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    int x1=x-height/2;
    int x2=x+height/2;
    int y1=y-height;
    int y2=y1;
    sdlutil::DrawTrangle(renderer,x,y,x1,y1,x2,y2,color.r,color.g,color.b);
}

}
