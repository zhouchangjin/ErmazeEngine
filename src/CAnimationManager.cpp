#include "CAnimationManager.h"

CAnimationManager::CAnimationManager()
{
    //ctor
}

CAnimationManager::~CAnimationManager()
{
    //dtor
}

void CAnimationManager::Init()
{
    m_sprite_db=CServiceLocator::GetService<CSpriteDB>
                (CServiceLocator::ServiceID::SPRITE_DB);
    m_image_db=CServiceLocator::GetService<CImageDB>
               (CServiceLocator::ServiceID::TEXTURE_DB);
    if(m_context)
    {
        m_particle_system.SetGameContext(m_context);
        m_particle_system.Init();
    }

}

void CAnimationManager::Update()
{

    for(size_t i=0; i<m_animation_list.size(); i++)
    {
        CAnimationItem& item= m_animation_list[i];
        CAnimationItem::AnimateType type=item.GetAnimateType();
        //if(item is dead) remove item
        int frame=m_frame-item.GetGlobalFrame();  //正常情况 uint32 a<b a-b>0,但这里不需要注意
        if(frame>item.GetEndFrame())
        {
            bool reset=item.GetResetPosition();
            if(reset)
            {
                CSpriteGameObject* obj=item.GetObject();
                if(obj)
                {
                    obj->SetX(item.GetStartLoc().x);
                    obj->SetY(item.GetStartLoc().y);
                    if(item.GetAnimateType()==
                            CAnimationItem::AnimateType::FLASH_SPRITE)
                    {
                        obj->SetAlpha(1.0f);
                    }
                    else if(item.GetAnimateType()==
                            CAnimationItem::AnimateType::SHINE_SPRITE)
                    {
                        obj->DisableMask();
                        obj->DisableOutline();
                    }
                    obj->UpdateDirection(item.GetActionName());
                }
            }
            m_animation_list.erase(m_animation_list.begin()+i);
            i--;
        }
        else
        {


            if(type==CAnimationItem::AnimateType::MOVE_SPRITE)
            {
                //TODO
                CSpriteGameObject* object=item.GetObject();
                //GE_LOG("====%s=-=%d==\n",item.GetActionName().c_str(),object->GetStep());
                int frame=m_frame-item.GetGlobalFrame();
                int f_start=item.GetStartFrame();
                int framespan=item.GetEndFrame()-item.GetStartFrame();
                if(frame>=f_start && frame<=item.GetEndFrame())
                {
                    int start_loc_x=item.GetStartLoc().x;
                    int start_loc_y=item.GetStartLoc().y;
                    int end_loc_x=item.GetEndLoc().x;
                    int end_loc_y=item.GetEndLoc().y;
                    int cur_loc_x=start_loc_x;
                    int cur_loc_y=start_loc_y;
                    if(framespan>0)
                    {
                        int dx=(end_loc_x-start_loc_x)*(frame-f_start)/framespan;
                        int dy=(end_loc_y-start_loc_y)*(frame-f_start)/framespan;
                        cur_loc_x+=dx;
                        cur_loc_y+=dy;
                    }
                    object->SetX(cur_loc_x);
                    object->SetY(cur_loc_y);
                }

            }
            else if(type==CAnimationItem::AnimateType::DISAPPEAR)
            {
                CSpriteGameObject* object=item.GetObject();
                object->PlayAction(item.GetActionName());
                int frame=m_frame-item.GetGlobalFrame();
                int f_start=item.GetStartFrame();
                int framespan=item.GetEndFrame()-item.GetStartFrame();
                if(frame>=f_start && frame<=item.GetEndFrame())
                {
                    float alpha=1.0f-((frame-f_start)*1.0f/framespan);
                    GE_LOG("%f=====\n",alpha);
                    object->SetAlpha(alpha);
                }
            }
            else if(type==CAnimationItem::AnimateType::FLASH_SPRITE)
            {
                CSpriteGameObject* object=item.GetObject();
                object->PlayAction(item.GetActionName());
                int frame=m_frame-item.GetGlobalFrame();
                int f_start=item.GetStartFrame();
                int framespan=item.GetEndFrame()-item.GetStartFrame();
                if(frame>=f_start && frame<=item.GetEndFrame())
                {
                    int start_loc_x=item.GetStartLoc().x;
                    int start_loc_y=item.GetStartLoc().y;
                    int end_loc_x=item.GetEndLoc().x;
                    int end_loc_y=item.GetEndLoc().y;
                    int cur_loc_x=start_loc_x;
                    int cur_loc_y=start_loc_y;
                    if(framespan>0)
                    {
                        int dx=(end_loc_x-start_loc_x)*(frame-f_start)/framespan;
                        int dy=(end_loc_y-start_loc_y)*(frame-f_start)/framespan;
                        cur_loc_x+=dx;
                        cur_loc_y+=dy;
                    }
                    if(frame%2==0)
                    {
                        object->SetAlpha(0.0f);
                    }
                    else
                    {
                        object->SetAlpha(1.0f);
                    }

                }

            }
            else if(type==CAnimationItem::AnimateType::SHINE_SPRITE)
            {

                CSpriteGameObject* object=item.GetObject();
                object->PlayAction(item.GetActionName());
                object->EnableOutline();
                object->EnableMask();
                int frame=m_frame-item.GetGlobalFrame();
                int f_start=item.GetStartFrame();
                int framespan=item.GetEndFrame()-item.GetStartFrame();
                if(frame>=f_start && frame<=item.GetEndFrame())
                {
                    int start_loc_x=item.GetStartLoc().x;
                    int start_loc_y=item.GetStartLoc().y;
                    int end_loc_x=item.GetEndLoc().x;
                    int end_loc_y=item.GetEndLoc().y;
                    int cur_loc_x=start_loc_x;
                    int cur_loc_y=start_loc_y;
                    if(framespan>0)
                    {
                        int dx=(end_loc_x-start_loc_x)*(frame-f_start)/framespan;
                        int dy=(end_loc_y-start_loc_y)*(frame-f_start)/framespan;
                        cur_loc_x+=dx;
                        cur_loc_y+=dy;
                    }
                    ge_common_struct::ge_adv_color white;
                    white.r=255;
                    white.g=255;
                    white.b=255;
                    ge_common_struct::ge_adv_color black;
                    black.r=0;
                    black.g=0;
                    black.b=0;
                    if(frame%2==0)
                    {
                        object->SetOutlineColor(white);
                        object->SetMaskColor(black);
                    }
                    else
                    {
                        object->SetOutlineColor(black);
                        object->SetMaskColor(white);
                    }

                }

            }
            else if(type==CAnimationItem::AnimateType::PARTICLE)
            {
                //update particle emitter position

            }
            else if(type==CAnimationItem::AnimateType::PROJECTILE)
            {
                //update projectile emitter position,nothing useful
                int frame=m_frame-item.GetGlobalFrame();
                int f_start=item.GetStartFrame();
                if(frame==f_start)
                {
                    CreateProjectile(item);
                }

            }
            else if(type==CAnimationItem::AnimateType::SHOW_VFX)
            {
                //TODO


            }
            else
            {

            }
        }



    }
    m_particle_system.Update();

}

void CAnimationManager::AddAnimateItem(CAnimationItem item)
{
    //if item is particle effect,create emmiter;
    if(item.GetAnimateType()==CAnimationItem::AnimateType::SHOW_VFX)
    {
        CSprite* sprite=m_sprite_db->GetSprite(item.GetSpriteName());
        item.SetVfxSprite(sprite);
        if(item.GetObject()!=nullptr)
        {
            CSpriteGameObject* obj=item.GetObject();
            int x=obj->GetX();
            int y=obj->GetY();
            int w=obj->GetObjectWidth();
            int h=obj->GetObjectHeight();
            int scale=obj->GetRenderScale();
            //TODO
            int effect_scale=2;
            int sw=sprite->GetSpriteSheet()->GetSpriteWidth();
            int sh=sprite->GetSpriteSheet()->GetSpriteHeight();
            int efx=x+w/2*scale-sw/2*effect_scale;
            int efy=y+h/2*scale-sh/2*effect_scale;
            item.SetStartLoc(ge_common_struct::ge_point(efx,efy));
            item.SetEndLoc(ge_common_struct::ge_point(efx,efy));
        }
    }
    else if(item.GetAnimateType()==CAnimationItem::AnimateType::MOVE_SPRITE)
    {
        CSpriteGameObject* obj=item.GetObject();
        if(obj!=nullptr)
        {
            int x=obj->GetX();
            int y=obj->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(x,y));
            std::string action_name=obj->GetCurrentAction();
            obj->ResetStep();
            obj->UpdateDirection(item.GetActionName());

            item.SetActionName(action_name);
        }
        CSpriteGameObject* target=item.GetTargetObject();
        if(target!=nullptr)
        {
            int x=target->GetX();
            int y=target->GetY();
            item.SetEndLoc(ge_common_struct::ge_point(x,y));
        }

        //DO NOTHING
    }
    else if(item.GetAnimateType()==CAnimationItem::AnimateType::PARTICLE)
    {
        //Create particle
        CSpriteGameObject* object=item.GetObject();
        CSpriteGameObject* tar_obj=item.GetTargetObject();
        if(object!=nullptr)
        {
            int px=object->GetX();
            int py=object->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(px,py));
        }
        if(tar_obj!=nullptr)
        {
            int px=tar_obj->GetX();
            int py=tar_obj->GetY();
            item.SetEndLoc(ge_common_struct::ge_point(px,py));
        }

    }
    else if(item.GetAnimateType()==CAnimationItem::AnimateType::PROJECTILE)
    {
        //Create projectile
        CSpriteGameObject* object=item.GetObject();
        CSpriteGameObject* tar_obj=item.GetTargetObject();
        if(object!=nullptr)
        {
            int px=object->GetX();
            int py=object->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(px,py));
        }
        if(tar_obj!=nullptr)
        {
            int px=tar_obj->GetX();
            int py=tar_obj->GetY();
            int width=tar_obj->GetObjectWidth()*tar_obj->GetRenderScale();
            int height=tar_obj->GetObjectHeight()*tar_obj->GetRenderScale();
            int shoot_loc_x=px+width/2;
            int shoot_loc_y=py+height/2;
            item.SetEndLoc(ge_common_struct::ge_point(shoot_loc_x,shoot_loc_y));
        }

    }
    else if(item.GetAnimateType()==CAnimationItem::AnimateType::TEXT_MOTION)
    {

        //sdlutil2::RenderText()
        CSpriteGameObject* obj=item.GetObject();
        if(obj!=nullptr)
        {
            int x=obj->GetX();
            int y=obj->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(x,y));
            item.SetEndLoc(ge_common_struct::ge_point(x,y-50));
        }


    }
    else if(item.GetAnimateType()==CAnimationItem::AnimateType::FLASH_SPRITE)
    {
        CSpriteGameObject* obj=item.GetObject();
        if(obj!=nullptr)
        {
            int x=obj->GetX();
            int y=obj->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(x,y));
            item.SetEndLoc(ge_common_struct::ge_point(x,y));
        }
    }
    else if(item.GetAnimateType()==CAnimationItem::AnimateType::SHINE_SPRITE)
    {
        CSpriteGameObject* obj=item.GetObject();
        if(obj!=nullptr)
        {
            int x=obj->GetX();
            int y=obj->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(x,y));
            item.SetEndLoc(ge_common_struct::ge_point(x,y));
        }

    }
    else if(item.GetAnimateType()==CAnimationItem::AnimateType::DISAPPEAR)
    {
        CSpriteGameObject* obj=item.GetObject();
        if(obj!=nullptr)
        {
            int x=obj->GetX();
            int y=obj->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(x,y));
            item.SetEndLoc(ge_common_struct::ge_point(x,y));
        }

    }
    item.SetGlobalFrame(m_frame);
    m_animation_list.push_back(item);


}

void CAnimationManager::Draw()
{
    //GE_LOG("========animate_item num=%d\n",m_animation_list.size());
    for(size_t i=0; i<m_animation_list.size(); i++)
    {
        CAnimationItem& item= m_animation_list[i];
        CAnimationItem::AnimateType type=item.GetAnimateType();
        if(type==CAnimationItem::AnimateType::SHOW_VFX)
        {
            std::string action_name=item.GetActionName();
            int frame=m_frame-item.GetGlobalFrame();
            int f_start=item.GetStartFrame();
            int framespan=item.GetEndFrame()-item.GetStartFrame();
            if(frame>=item.GetStartFrame() && frame<=item.GetEndFrame())
            {

                int step=frame/item.GetFrameRate();
                CSprite* vfxsprite=item.GetVfxSprite();
                int cnt=vfxsprite->ActionStepCnt(action_name);
                if(!item.IsLoop())
                {

                    if(step>=cnt)
                    {
                        step=cnt-1;
                    }
                }
                int idx=vfxsprite->PlayAction(action_name,step);

                int start_loc_x=item.GetStartLoc().x;
                int start_loc_y=item.GetStartLoc().y;
                int end_loc_x=item.GetEndLoc().x;
                int end_loc_y=item.GetEndLoc().y;
                int cur_loc_x=start_loc_x;
                int cur_loc_y=start_loc_y;
                if(framespan>0)
                {
                    int dx=(end_loc_x-start_loc_x)*(frame-f_start)/framespan;
                    int dy=(end_loc_y-start_loc_y)*(frame-f_start)/framespan;
                    cur_loc_x+=dx;
                    cur_loc_y+=dy;
                }
                sdlutil2::RenderSprite(m_context,vfxsprite,
                                       cur_loc_x,cur_loc_y,idx,2);
            }

        }
        else if(type==CAnimationItem::AnimateType::TEXT_MOTION)
        {
            int frame=m_frame-item.GetGlobalFrame();
            int f_start=item.GetStartFrame();
            int framespan=item.GetEndFrame()-item.GetStartFrame();
            if(frame>=f_start && frame<=item.GetEndFrame())
            {
                int start_loc_x=item.GetStartLoc().x;
                int start_loc_y=item.GetStartLoc().y;
                int end_loc_x=item.GetEndLoc().x;
                int end_loc_y=item.GetEndLoc().y;
                int cur_loc_x=start_loc_x;
                int cur_loc_y=start_loc_y;
                if(framespan>0)
                {
                    int dx=(end_loc_x-start_loc_x)*(frame-f_start)/framespan;
                    int dy=(end_loc_y-start_loc_y)*(frame-f_start)/framespan;
                    cur_loc_x+=dx;
                    cur_loc_y+=dy;
                }
                //font size is not using currently
                ge_common_struct::ge_color color=item.GetFontColor();
                //GE_LOG("%d,%d,%d,\n",color.r,color.g,color.b);
                sdlutil2::RenderText(m_context,m_context->GetFont(),cur_loc_x,
                                     cur_loc_y,item.GetText(),
                                     color);
            }
        }

    }
    m_particle_system.Draw();
    m_frame++;
}


void CAnimationManager::CreateProjectile(CAnimationItem item)
{
    std::string texture_name=item.GetSpriteName();
    int x=item.GetStartLoc().x;
    int y=item.GetStartLoc().y;
    int tx=item.GetEndLoc().x;
    int ty=item.GetEndLoc().y;
    int end_frame=item.GetEndFrame();
    int start_frame=item.GetStartFrame();
    int span=end_frame-start_frame;
    int vx=(tx-x)/span;
    int vy=(ty-y)/span;
    CProjectileEmitter* projectile_emitter=new CProjectileEmitter();
    m_particle_system.AddEmitter(projectile_emitter);
    CProjectile pj;
    pj.SetPosX(x);
    pj.SetPosY(y);
    pj.SetAX(0);
    pj.SetAY(0);
    pj.SetVelocityX(vx);
    pj.SetVelocityY(vy);
    pj.SetLife(span+10);
    pj.SetTextureName(texture_name);
    projectile_emitter->SetProjectile(pj);
    projectile_emitter->Init();

}
