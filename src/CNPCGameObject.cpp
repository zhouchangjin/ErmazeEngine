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
