#include "CPCSpriteGameObject.h"

CPCSpriteGameObject::CPCSpriteGameObject(CSprite* object):CSpriteGameObject(object)
{
    //ctor
}

CPCSpriteGameObject::~CPCSpriteGameObject()
{
    //dtor
}

bool CPCSpriteGameObject::IsActive(){
    return m_action_active;
}

void CPCSpriteGameObject::ComsumeAction(){
    m_action_active=false;
}

void CPCSpriteGameObject::ProcessInput(CInputEvent event)
{
    int state=event.GetCurrentSubState();
    if(state==0)
    {
        ge_common_struct::key_event_type type=event.GetInput().get_top_event().event;
        if(type==ge_common_struct::key_event_type::KEY_LEFT)
        {
            MoveLeftward(16);
        }
        else if(type==ge_common_struct::key_event_type::KEY_RIGHT)
        {
            MoveRightward(16);
        }
        else if(type==ge_common_struct::key_event_type::KEY_UP)
        {
            MoveUpward(16);
        }
        else if(type==ge_common_struct::key_event_type::KEY_DOWN)
        {
            MoveDownward(16);
        }else if(type==ge_common_struct::key_event_type::KEY_SPACE){
            m_action_active=true;
        }
    }

}
