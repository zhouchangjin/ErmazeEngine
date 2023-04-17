#include "Render_Util.h"
//该工具方法可以被面向对象的Service类取代
#include "CSdlGameContext.h"
namespace sdlutil2
{
ge_common_struct::dom_node* CreateNodeFromTemplate(
    ge_common_struct::dom_node* temp,
    ge_common_struct::dom_node* parent,
    int context_id)
{
    ge_common_struct::dom_node* node=new ge_common_struct::dom_node();
    node->attributes=temp->attributes;
    node->box=temp->box;
    node->child_layout=temp->child_layout;
    node->col=temp->col;
    node->ele_name=temp->ele_name;
    node->list_name=temp->list_name;
    node->obj_id=context_id;
    node->parent_node=parent;
    node->row=temp->row;
    node->style=temp->style;
    node->template_text=temp->template_text;
    node->use_template=temp->use_template;
    node->text=temp->text;
    node->template_node=temp;
    if(temp->children.size()>0)
    {
        for(size_t i=0; i<temp->children.size(); i++)
        {
            ge_common_struct::dom_node* childtemp=temp->children[i];
            ge_common_struct::dom_node* child=CreateNodeFromTemplate(childtemp,
                                              node,context_id);
            node->children.push_back(child);
        }
    }
    return node;
}

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

 SDL_Window* GetWindow(CGameContext* p_context){
    CSdlGameContext* context=(CSdlGameContext*)p_context;
    SDL_Window* window=context->GetWindow();
    return window;
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

void MaskSprite(CGameContext* p_context,CSprite* sprite,int screenx,
                int screeny,int sprite_idx,int scale
                ,ge_common_struct::ge_adv_color mask_color,ge_common_struct::ge_color ignore_color){

    CSpriteSheet* spritesheet=sprite->GetSpriteSheet();
    SDL_Window* window=GetWindow(p_context);
    SDL_Renderer* renderer=GetRenderer(p_context);

    void *surface=spritesheet->GetSurface();

    int sprite_w=spritesheet->GetSpriteWidth();
    int sprite_h=spritesheet->GetSpriteHeight();

    if(!surface){
        std::string path=spritesheet->GetSpritePath();
        SDL_Surface* sdlsurface=sdlutil::LoadFormatSurface(path,window);
        spritesheet->SetSurface(sdlsurface);
        surface=spritesheet->GetSurface();
    }

    SDL_Surface* fsurface=(SDL_Surface*)surface;
    SDL_Texture* msktexture = SDL_CreateTexture(renderer,
                                SDL_GetWindowPixelFormat(window),
                            SDL_TEXTUREACCESS_STREAMING,
                            sprite_w, sprite_h);
    int pitch;
    void * pixels;
    SDL_LockTexture(msktexture,NULL,&pixels, &pitch );
    ge_common_struct::ge_rect rect=sprite->GetRectByIdx(sprite_idx);

    uint8_t* surface_pixels=(uint8_t*)fsurface->pixels;
    uint8_t* texture_pixels=(uint8_t*)pixels;
    int surface_pitch=fsurface->pitch;
    SDL_Rect sdlrect;
    sdlrect.x=0;
    sdlrect.y=0;
    sdlrect.w=sprite_w;
    sdlrect.h=sprite_h;
    int base_x=rect.x;
    int base_y=rect.y;
    for(int i=0;i<sprite_h;i++){
        for(int j=0;j<sprite_w;j++){
            int y=i+base_y;
            int x=j+base_x;
            int b=surface_pixels[y*surface_pitch+x*4];
            int g=surface_pixels[y*surface_pitch+x*4+1];
            int r=surface_pixels[y*surface_pitch+x*4+2];
            int a=surface_pixels[y*surface_pitch+x*4+3];
            if(ignore_color.r==r && ignore_color.g==g && ignore_color.b==b){
                texture_pixels[i*pitch+j*4]=b;
                texture_pixels[i*pitch+j*4+1]=g;
                texture_pixels[i*pitch+j*4+2]=r;
                texture_pixels[i*pitch+j*4+3]=a;
            }else{
                texture_pixels[i*pitch+j*4]=b+mask_color.b;
                texture_pixels[i*pitch+j*4+1]=g+mask_color.g;
                texture_pixels[i*pitch+j*4+2]=r+mask_color.r;
                texture_pixels[i*pitch+j*4+3]=a+mask_color.a;
            }
        }
    }

    SDL_UnlockTexture(msktexture);
    sdlutil::RenderTexture(screenx,screeny,&sdlrect,msktexture,renderer,scale);
    SDL_DestroyTexture(msktexture);
}

void RenderSprite(CGameContext* p_context,CSprite* sprite,int screenx,
                  int screeny,int sprite_idx,int scale,int alpha)
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
    if(alpha<=255 && alpha>=0)
    {

        sdlutil::EnableAndSetTextureAlpha(sdl_texture,alpha);
    }
    else if(alpha<0)
    {
        return;
    }
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
    sdlutil::FillTrangle(renderer,triangle.p0.x,triangle.p0.y,
                         triangle.p1.x,triangle.p1.y,
                         triangle.p2.x,triangle.p2.y,color.r,color.g,color.b);
}


void RenderText(CGameContext* p_context,void* font,int x,int y,
                std::string textureText,ge_common_struct::ge_color text_color,
                int scale)
{
    if(textureText.compare("")==0)
    {
        return;
    }
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
    dialog.TextUpdate();
}

void DrawChoiceDialog(CGameContext* p_context,CChoiceDialog& dialog)
{
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
        CChoiceDialog* choice_dialog=dialog.GetChoiceDialog();
        DrawChoiceDialog(p_context,*choice_dialog);
    }
}


void UpdateDomRect(ge_common_struct::dom_node* node,
                   ge_common_struct::ge_rect parent_rect,
                   int &offsetx,int &offsety,int &paint_height)
{
    ge_common_struct::ui_layout layout;
    int actual_width=0;
    std::string text=node->text;
    int content_height=0;
    int word_cnt=node->style.is_icon?0:ge_str_utilities::Utf8Strlen(text);

    if(node->parent_node==nullptr)
    {
        layout=ge_common_struct::ui_layout::NULL_LAYOUT;
    }
    else
    {
        layout=node->parent_node->child_layout;
    }
    ge_common_struct::ge_rect rc=node->style.client_rect;
    if(rc.w==0)
    {
        //宽度受内容限制
        if(node->children.size()==0)
        {
            if(node->style.is_icon)
            {
            }
            else
            {
                actual_width=node->style.font_size*word_cnt;
            }
        }
        else
        {
            if(node->parent_node!=nullptr)
            {
                if(node->style.is_percentage)
                {
                    rc.w=100;
                }
                else
                {
                    actual_width=parent_rect.w;
                }
            }
        }
    }
    if(layout==ge_common_struct::ui_layout::NULL_LAYOUT)
    {

        if(!node->style.position_is_absolute)
        {
            if(node->style.is_percentage)
            {
                node->box.x=parent_rect.x+rc.x*parent_rect.w/100;
                node->box.y=parent_rect.y+rc.y*parent_rect.h/100;
                node->box.w=parent_rect.w*rc.w/100;
                node->box.h=parent_rect.h*rc.h/100;
            }
            else
            {
                node->box.x=parent_rect.x+rc.x;
                node->box.y=parent_rect.y+rc.y;
                node->box.w=rc.w==0?actual_width:rc.w;
                node->box.h=rc.h;
            }
        }
        else
        {
            if(node->style.is_percentage)
            {
                node->box.x=rc.x*parent_rect.w/100;
                node->box.y=rc.y*parent_rect.h/100;
                node->box.w=rc.w*parent_rect.w/100;
                node->box.h=rc.h*parent_rect.h/100;
            }
            else
            {
                node->box.x=rc.x;
                node->box.y=rc.y;
                node->box.w=rc.w==0?actual_width:rc.w;
                //node->box.w=rc.w;
                node->box.h=rc.h;
            }
        }

        if(node->box.h>paint_height)
        {
            paint_height=node->box.h;
        }

    }
    else
    {
        if(node->style.is_percentage)
        {
            node->box.w=rc.w*parent_rect.w/100;
            node->box.h=rc.h*parent_rect.h/100;
            //GE_LOG("%s %s (parent_wnd_height=%d  rect_percent_height=%d)\n",node->ele_name.c_str(),node->text.c_str(),parent_rect.h,rc.h);
        }
        else
        {
            node->box.w=rc.w==0?actual_width:rc.w;
            node->box.h=rc.h;

            //GE_LOG("%s %s (parent_wnd_height=%d  rect_height=%d)\n",node->ele_name.c_str(),node->text.c_str(),parent_rect.h,rc.h);
        }

        node->box.x=parent_rect.x+offsetx;

        if(node->box.x+node->box.w>parent_rect.x+parent_rect.w)
        {
            node->box.x=parent_rect.x;
            offsetx=node->box.w;
            offsety+=paint_height;
            node->box.y=parent_rect.y+offsety;
            paint_height=node->box.h;
        }
        else
        {
            offsetx=node->box.x+node->box.w-parent_rect.x;
            node->box.y=parent_rect.y+offsety;
            if(node->box.h>paint_height)
            {
                paint_height=node->box.h;
            }
        }
    }

    size_t child_cnt=node->children.size();
    int offx_new=0;
    int offy_new=0;
    int paint_h_new=0;
    for(size_t j=0; j<child_cnt; j++)
    {
        ge_common_struct::dom_node* cnode=node->children[j];
        ge_common_struct::ge_rect p_rectnew; //p_rect必须是非百分比的参数
        //微软的盒模型 border计算入宽高的一部分
        p_rectnew.x=node->box.x+node->style.border_width;
        p_rectnew.y=node->box.y+node->style.border_width;
        p_rectnew.w=node->box.w-node->style.border_width*2;
        p_rectnew.h=node->box.h-node->style.border_width*2;
        UpdateDomRect(cnode,p_rectnew,offx_new,offy_new,paint_h_new);
    }

    if(node->box.h<=0)
    {
        if(word_cnt>0)
        {
            int char_cnt_per_line=node->box.w/node->style.font_size;
            int line_cnt=(word_cnt-1)/char_cnt_per_line+1;
            content_height=line_cnt*node->style.line_height;
        }
        int actual_height=offy_new+content_height+paint_h_new
                          +node->style.border_width*2;
        node->box.h=actual_height;
        if(node->box.h>paint_height)
        {
            paint_height=node->box.h;
        }
    }

}

void UpdateDomRect(ge_common_struct::dom_node* node,
                   ge_common_struct::ge_rect parent_rect)
{
    int offsetx=0;
    int offsety=0;
    int paint_height=0;
    UpdateDomRect(node,parent_rect,offsetx,offsety,paint_height);
}

void DrawDomNode(CGameContext* p_context,ge_common_struct::dom_node* node,CImageDB* imagedb)
{
    ge_common_struct::box_style style=node->style;
    if(!style.visibility)
    {
        return;
    }
    ge_common_struct::ge_adv_color border_color=style.border_color;
    ge_common_struct::ge_adv_color bg_color=style.background_color;
    ge_common_struct::ge_color f_color=style.font_color;
    int font_size=style.font_size;
    int line_height=style.line_height;
    size_t child_cnt=node->children.size();

    //
    ge_common_struct::ge_rect box_rect=node->box;
    if(style.out_radius>0)
    {
        border_color.a=border_color.a*bg_color.a/255;
        FillRoundRect(p_context,box_rect,style.out_radius,border_color);
        if(style.draw_shape)
        {
            ge_common_struct::ge_adv_color scolor= {0,0,0,255};
            DrawRoundRect(p_context,box_rect,style.out_radius,scolor);
        }
    }
    else
    {
        //FillRect(p_context,box_rect,border_color.r,border_color.g,border_color.b,border_color.a);
        DrawBorder(p_context,box_rect,border_color,
                   ge_common_struct::ge_sides(style.border_width));
    }
    ge_common_struct::ge_rect rect;
    rect.x=box_rect.x+style.border_width;
    rect.y=box_rect.y+style.border_width;
    rect.w=box_rect.w-2*style.border_width;
    rect.h=box_rect.h-2*style.border_width;
    if(node->child_layout!=ge_common_struct::ui_layout::GRID_LAYOUT)
    {
        FillRect(p_context,rect,bg_color.r,bg_color.g,bg_color.b,bg_color.a);
        if(node->style.background_texture)
        {
            std::string texture_name=node->style.texture_name;
            int scale=node->style.texture_scale;
            FillRectTexture(p_context,rect,imagedb->GetTiledTexture(texture_name),texture_name,scale);
        }
    }
    if(child_cnt>0)
    {
        std::vector<ge_common_struct::dom_node*> children=node->children;
        for(size_t i=0; i<child_cnt; i++)
        {
            DrawDomNode(p_context,children[i],imagedb);
        }
    }
    else
    {
        ge_common_struct::ge_sides padding=style.padding;
        ge_common_struct::ge_rect actual_rect;
        actual_rect.x=rect.x+padding.left;
        actual_rect.y=rect.y+padding.top;
        actual_rect.w=rect.w-padding.left-padding.right;
        actual_rect.h=rect.h-padding.top-padding.bottom;
        ge_common_struct::text_align align=style.align;
        if(node->style.is_icon)
        {
            std::string icon_name=node->text;
            CTexture icon=imagedb->GetTexture(icon_name);
            DrawTexture2(p_context,actual_rect,icon);
        }
        else
        {

            if(node->has_attribute("indicator_texture"))
            {
                uint32_t frame=node->frame;
                std::string texture_name=node->attr_str("indicator_texture");
                std::string show_text=ge_str_utilities::TrimStr(node->text);
                if(texture_name.compare("")!=0 && show_text.compare(" ")!=0
                   && show_text.compare("")!=0)
                {
                    CTexture indicator=imagedb->GetTexture(texture_name);

                    if(indicator)
                    {
                        int width=indicator.GetTiledTexture()->
                                  GetSpriteSheet()->GetSpriteWidth();
                        int height=indicator.GetTiledTexture()->
                                   GetSpriteSheet()->GetSpriteHeight();
                        ge_common_struct::ge_rect indicator_rect;
                        int cnt=indicator.GetTiledTexture()
                                ->ActionStepCnt(texture_name);
                        int offsetY=0;
                        if(cnt==1){
                            offsetY+=(frame/2)%4;
                        }
                        indicator_rect.w=width;
                        indicator_rect.h=height;
                        indicator_rect.x=actual_rect.x+actual_rect.w
                                         -width-padding.right;
                        indicator_rect.y=actual_rect.y+actual_rect.h
                                         -height-padding.bottom+offsetY;
                        DrawTexture2(p_context,indicator_rect,indicator);
                    }

                }

            }
            //TODO 当文字超过一行
            if(align==ge_common_struct::text_align::LEFT)
            {
                int x=actual_rect.x;
                int y=actual_rect.y;
                RenderText(p_context,p_context->GetFont(),
                           x,y,node->text,f_color);
            }
            else if(align==ge_common_struct::text_align::CENTER)
            {
                int charcnt=ge_str_utilities::Utf8Strlen(node->text);
                int margin_l=(actual_rect.w-charcnt*font_size)/2;
                int margin_t=line_height-font_size;
                if(margin_t<0)
                {
                    margin_t=0;
                }
                if(margin_l<0)
                {
                    margin_l=0;
                }
                int x=actual_rect.x+margin_l;
                int y=actual_rect.y+margin_t;
                RenderText(p_context,p_context->GetFont(),
                           x,y,node->text,f_color);
            }
            else if(align==ge_common_struct::text_align::RIGHT)
            {
                int charcnt=ge_str_utilities::Utf8Strlen(node->text);
                int x=actual_rect.x+actual_rect.w-font_size*charcnt;
                int y=actual_rect.y;
                RenderText(p_context,p_context->GetFont(),x,y,node->text,f_color);
            }

        }

    }

}

void DrawWindow(CGameContext* p_context,CGameWindow& window)
{
    std::string title=window.GetTitle();
    int length=ge_str_utilities::Utf8Strlen(title);
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
    sdlutil::FillTrangle(renderer,x,y,x1,y1,x2,y2,color.r,color.g,color.b);
}

void FillRoundRect(CGameContext* p_context,ge_common_struct::ge_rect rect,
                   int radius,ge_common_struct::ge_adv_color color)
{
    SDL_Renderer * renderer=GetRenderer(p_context);
    sdlutil::FillRoundRect(renderer,rect.x,rect.y,rect.w,rect.h,radius,
                           color.r,color.g,color.b,color.a);
}

void DrawRoundRect(CGameContext* p_context,ge_common_struct::ge_rect rect,
                   int radius,ge_common_struct::ge_adv_color color)
{

    SDL_Renderer * renderer=GetRenderer(p_context);
    sdlutil::DrawRoundRect(renderer,rect.x,rect.y,rect.w,rect.h,radius,
                           color.r,color.g,color.b,color.a);
}

void DrawTexture2(CGameContext* p_context,ge_common_struct::ge_rect rect
                  ,CTexture texture,int scale)
{
    CTiledTexture* ti=texture.GetTiledTexture();
    if(ti)
    {
        int idx=texture.GetIdx();
        RenderSprite(p_context,ti,rect.x,rect.y,idx,scale);
    }

}

void DrawTexture(CGameContext* p_context,ge_common_struct::dom_node* node
                 ,unsigned int pointer_pos,CTiledTexture texture,std::string name,
                 int offsetx,int offsety,int scale)
{
    if(texture.GetSpriteSheet())
    {
        if(pointer_pos<node->children.size())
        {
            ge_common_struct::dom_node* cnode=node->children[pointer_pos];
            int screenx=cnode->box.x+offsetx;
            int screeny=cnode->box.y+offsety;
            int pos=texture.GetTexturePos(name);
            RenderSprite(p_context,&texture,screenx,screeny,pos,scale);
        }
        else
        {
            size_t cnt=node->children.size();
            //TODO 如果是有行列排列则下面代码才有效
            if(cnt>0)
            {
                int p_parent=pointer_pos%cnt;
                ge_common_struct::dom_node* cnode=node->children[p_parent];
                size_t ccnt=cnode->children.size();
                size_t idx=pointer_pos/cnt;
                if(idx<ccnt)
                {
                    ge_common_struct::dom_node* ccnode=cnode->children[idx];
                    int screenx=ccnode->box.x+offsetx;
                    int screeny=ccnode->box.y+offsety;
                    int pos=texture.GetTexturePos(name);
                    RenderSprite(p_context,&texture,screenx,screeny,pos,scale);
                }

            }

        }
    }
}

void DrawBorder(CGameContext* context,ge_common_struct::ge_rect rect,
                ge_common_struct::ge_adv_color color,
                ge_common_struct::ge_sides border)
{
    ge_common_struct::ge_rect top_rect=GetBorderTop(rect,border.top);
    FillRect(context,top_rect,color.r,color.g,color.b,color.a);
    ge_common_struct::ge_rect bottom_rect=GetBorderBottom(rect,border.bottom);
    FillRect(context,bottom_rect,color.r,color.g,color.b,color.a);
    ge_common_struct::ge_rect left_rect=GetBorderLeft(rect,border.left);
    FillRect(context,left_rect,color.r,color.g,color.b,color.a);
    ge_common_struct::ge_rect right_rect=GetBorderRight(rect,border.right);
    FillRect(context,right_rect,color.r,color.g,color.b,color.a);

}

void FillRectTexture(CGameContext* p_context,ge_common_struct::ge_rect rect,
                     CTiledTexture tiled_texture,std::string texture_name,
                     int scale)
{
    int pos=tiled_texture.GetTexturePos(texture_name);
    CSpriteSheet* spritesheet=tiled_texture.GetSpriteSheet();
    void * texture=spritesheet->GetTexture();
    if(!texture)
    {
        std::string path=spritesheet->GetSpritePath();
        spritesheet->SetTexture(LoadPngTexture(path,p_context));
        texture=spritesheet->GetTexture();
    }
    SDL_Texture* sdl_texture=(SDL_Texture* )texture;
    SDL_Renderer * renderer=GetRenderer(p_context);
    ge_common_struct::ge_rect srect=spritesheet->GetRectByIdx(pos);
    SDL_Rect sdlrect=TransformRect(srect);
    sdlutil::FillRectWithTexture(renderer,rect.x,rect.y,rect.w,rect.h,
                                 sdl_texture,sdlrect.x,sdlrect.y,
                                 sdlrect.w,sdlrect.h,scale);
}

void UpdateDomNode(ge_common_struct::dom_node* node,CGameDatabase* gamedb,
                   int context_obj,int page_start)
{
    node->frame++;
    if(node->list_template!=nullptr)
    {
        //当前节点是一个列表
        ge_common_struct::FreeDomVector(node->children);

        ge_common_struct::dom_node* list_template=node->list_template;

        std::vector<ge_common_struct::dom_node*>& children=
            list_template->children;

        std::string list_name=node->list_template->list_name;
        std::vector<int> ids=gamedb->GetListObjectIds(list_name);
        uint32_t max_size=ids.size();
        if(node->enable_page)
        {
            max_size=page_start+node->page_size;
            if(max_size>ids.size())
            {
                max_size=ids.size();
            }
        }
        for(size_t j=page_start; j<max_size; j++)
        {
            int id=ids[j];
            ge_common_struct::dom_node* list_item=
                new ge_common_struct::dom_node();
            node->children.push_back(list_item);
            list_item->ele_name="list_container";
            list_item->obj_id=id;
            list_item->child_seq_no=j;

            list_item->child_layout=list_template->child_layout;
            list_item->col=list_template->col;
            list_item->row=list_template->row;

            list_item->style.border_color.a=0;
            list_item->style.background_color.a=0;
            list_item->parent_node=node;
            list_item->style.is_percentage=true;
            list_item->style.client_rect.h=list_template->style.client_rect.h;
            list_item->style.client_rect.w=list_template->style.client_rect.w;
            for(size_t i=0; i<children.size(); i++)
            {
                ge_common_struct::dom_node* temp=children[i];
                ge_common_struct::dom_node* child=
                    CreateNodeFromTemplate(temp,list_item,id);
                list_item->children.push_back(child);
            }
        }
    }
    if(node->obj_id>=0)
    {
        context_obj=node->obj_id;
    }
    if(node->children.size()>0)
    {
        for(size_t i=0; i<node->children.size(); i++)
        {
            UpdateDomNode(node->children[i],gamedb,context_obj,page_start);
        }
    }
    else
    {
        if(node->has_attribute("show_indicator")
                && node->has_attribute("indicator"))
        {
            std::string indicator=node->attr_str("indicator");
            std::string text_prop=gamedb->GetTextData(indicator);
            node->attr_str("indicator_texture",text_prop);
        }
        if(node->use_template && context_obj>=0)
        {
            std::string template_text=node->template_text;
            if(node->template_node!=nullptr)
            {
                std::vector<std::string>& var_list=node->template_node->var_list;
                for(auto prop_name:var_list)
                {
                    std::string replace_str="{"+prop_name+"}";
                    std::string obj_type=gamedb->GetObjectType(context_obj);
                    CGameDatabase::DataType type=
                        gamedb->GetPropType(obj_type,prop_name);
                    std::string value="";
                    if(prop_name.compare("label")==0)
                    {
                        value=gamedb->GetObjectLabel(context_obj);
                    }
                    else if(type==CGameDatabase::DataType::INTEGER)
                    {
                        int intValue=gamedb->GetObjectData(context_obj
                                                           ,prop_name);
                        value=std::to_string(intValue);
                    }
                    else if(type==CGameDatabase::DataType::TEXT)
                    {
                        value=gamedb->GetObjectText(context_obj,prop_name);

                    }
                    else if(type==CGameDatabase::DataType::ICON_ID)
                    {
                        value=gamedb->GetObjectText(context_obj,prop_name);
                    }
                    ge_str_utilities::ReplaceAll(template_text,replace_str,value);
                }
                node->text=template_text;
            }
        }
        else if(node->use_template && context_obj<0)
        {
            std::string template_text=node->template_text;
            std::vector<std::string>& var_list=node->var_list;
            for(auto prop_name:var_list)
            {
                std::string replace_str="{"+prop_name+"}";
                std::string value=gamedb->GetTextData(prop_name);
                ge_str_utilities::ReplaceAll(template_text,replace_str,value);
            }
            node->text=template_text;
        }

    }


}

ge_common_struct::dom_node* GetPageDom(ge_common_struct::dom_node* node)
{

    if(node->enable_page)
    {
        return node;
    }
    else
    {
        std::vector<ge_common_struct::dom_node*> children=node->children;
        for(size_t i=0; i<children.size(); i++)
        {
            ge_common_struct::dom_node* child=children[i];
            ge_common_struct::dom_node* result=GetPageDom(child);
            if(result)
            {
                return result;
            }
        }
        return nullptr;
    }
}

int GetMaxPageCntOfDomNode(ge_common_struct::dom_node* node
                           ,CGameDatabase* gamedb)
{
    ge_common_struct::dom_node* page_node=GetPageDom(node);
    if(page_node)
    {
        std::vector<int> ids=gamedb->GetListObjectIds(page_node->list_template->list_name);
        return ids.size();
    }
    else
    {
        return 0;
    }
}

void RenderParticles(CGameContext* p_context,std::vector<CParticle*> particles,
                     CTiledTexture tiledtexture)
{
    for(size_t i=0; i<particles.size(); i++)
    {
        CParticle* p=particles[i];
        if(!p->IsDead())
        {
            std::string texture_name=p->GetTextureName();
            /**
            if(particles.size()==1){
               GE_LOG("=======%s\n",texture_name.c_str());
            }
            **/
            int pos=tiledtexture.GetTexturePos(texture_name);
            RenderSprite(p_context,&tiledtexture,p->GetX(),p->GetY()
                         ,pos,p->GetSize());
        }
    }
}

}
