#include "CNPCGameObject.h"

CNPCGameObject::CNPCGameObject(CSprite* object):CSpriteGameObject(object)
{
    m_move_speed=2;
}

CNPCGameObject::~CNPCGameObject()
{
    //dtor
}

void CNPCGameObject::Move()
{
    m_frame_counter++;
    if(m_frame_counter%m_walking_pace==0)
    {

        if(m_walking_mode==ge_common_struct::npc_move_type::NPC_HALT)
        {
            Step();
        }else if(m_walking_mode==ge_common_struct::npc_move_type::NPC_RANDOM){
            int r=rand()%4;
            //GE_LOG("---%d\n",r);
            if(r==0){
                MoveDownward(16);
            }else if(r==1){
                MoveUpward(16);
            }else if(r==2){
                MoveLeftward(16);
            }else{
                MoveRightward(16);
            }
        }

    }
}

void CNPCGameObject::OnAction(CActionEvent event){
    int direction=event.GetActionDirection();
    ge_common_struct::action_source source=(ge_common_struct
                                            ::action_source)direction;
    if(source==ge_common_struct::action_source::FACE_DOWN){
        UpdateDirection("upward");
    }else if(source==ge_common_struct::action_source::FACE_UP){
        UpdateDirection("downward");
    }else if(source==ge_common_struct::action_source::FACE_RIGHT){
        UpdateDirection("leftward");
    }else if(source==ge_common_struct::action_source::FACE_LEFT){
        UpdateDirection("rightward");
    }
}
