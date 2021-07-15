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
    if(m_context){
        m_particle_system.SetGameContext(m_context);
        m_particle_system.Init();
    }

}

void CAnimationManager::Update()
{

    for(size_t i=0; i<m_animation_list.size(); i++)
    {
        AnimationItem& item= m_animation_list[i];
        AnimationItem::AnimateType type=item.GetAnimateType();
        //if(item is dead) remove item
        int frame=m_frame-item.GetGlobalFrame();
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
                }
            }
            m_animation_list.erase(m_animation_list.begin()+i);
            i--;
        }
        else
        {


            if(type==AnimationItem::AnimateType::MOVE_SPRITE)
            {
                //TODO
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
                    object->SetX(cur_loc_x);
                    object->SetY(cur_loc_y);
                }

            }
            else if(type==AnimationItem::AnimateType::FLASH_SPRITE)
            {
                CSpriteGameObject* object=item.GetObject();
                object->PlayAction(item.GetActionName());
                int frame=m_frame-item.GetGlobalFrame();
                int f_start=item.GetStartFrame();
                int framespan=item.GetEndFrame()-item.GetStartFrame();
                if(frame>=f_start && frame<=item.GetEndFrame()){
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
                    if(frame%2==0){
                        object->SetX(-10000);
                        object->SetY(-10000);
                    }else{
                        object->SetX(cur_loc_x);
                        object->SetY(cur_loc_y);
                    }

                }

            }
            else if(type==AnimationItem::AnimateType::PARTICLE)
            {
                //update particle emitter position

            }
            else if(type==AnimationItem::AnimateType::PROJECTILE)
            {
                //update projectile emitter position,nothing useful

            }
            else if(type==AnimationItem::AnimateType::SHOW_VFX)
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

void CAnimationManager::AddAnimateItem(AnimationItem item)
{
    //if item is particle effect,create emmiter;
    if(item.GetAnimateType()==AnimationItem::AnimateType::SHOW_VFX)
    {
        CSprite* sprite=m_sprite_db->GetSprite(item.GetSpriteName());
        item.SetVfxSprite(sprite);
        if(item.GetObject()!=nullptr)
        {
            CSpriteGameObject* obj=item.GetObject();
            int x=obj->GetX();
            int y=obj->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(x,y));
            item.SetEndLoc(ge_common_struct::ge_point(x,y));
        }
    }
    else if(item.GetAnimateType()==AnimationItem::AnimateType::MOVE_SPRITE)
    {
        CSpriteGameObject* obj=item.GetObject();
        if(obj!=nullptr)
        {
            int x=obj->GetX();
            int y=obj->GetY();
            item.SetStartLoc(ge_common_struct::ge_point(x,y));
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
    else if(item.GetAnimateType()==AnimationItem::AnimateType::PARTICLE)
    {
        //Create particle
    }
    else if(item.GetAnimateType()==AnimationItem::AnimateType::PROJECTILE)
    {
        //Create projectile

    }
    else if(item.GetAnimateType()==AnimationItem::AnimateType::TEXT_MOTION)
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


    }else if(item.GetAnimateType()==AnimationItem::AnimateType::FLASH_SPRITE){
        CSpriteGameObject* obj=item.GetObject();
        if(obj!=nullptr){
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
        AnimationItem& item= m_animation_list[i];
        AnimationItem::AnimateType type=item.GetAnimateType();
        if(type==AnimationItem::AnimateType::SHOW_VFX)
        {
            std::string action_name=item.GetActionName();
            int frame=m_frame-item.GetGlobalFrame();
            int f_start=item.GetStartFrame();
            int framespan=item.GetEndFrame()-item.GetStartFrame();
            if(frame>=item.GetStartFrame() && frame<=item.GetEndFrame())
            {
                int idx=item.GetVfxSprite()->PlayAction(action_name,frame);
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
                sdlutil2::RenderSprite(m_context,item.GetVfxSprite(),
                                       cur_loc_x,cur_loc_y,idx,2);
            }

        }
        else if(type==AnimationItem::AnimateType::TEXT_MOTION)
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
                sdlutil2::RenderText(m_context,m_context->GetFont(),cur_loc_x,
                                     cur_loc_y,item.GetText(),
                                     item.GetFontColor());
            }
        }

    }
    m_particle_system.Update();
    m_frame++;
}
