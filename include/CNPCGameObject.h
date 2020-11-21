#ifndef CNPCGAMEOBJECT_H
#define CNPCGAMEOBJECT_H

#include <CSpriteGameObject.h>
#include <IDefaultActionEventProcess.h>

class CNPCGameObject : public CSpriteGameObject,public IDefaultActionEventProcess
{
    public:
        CNPCGameObject(CSprite* sprite);
        virtual ~CNPCGameObject();
        void SetWalkingMode(ge_common_struct::npc_move_type mode)
        {m_walking_mode=mode;};
        void Move();
        void OnAction(CActionEvent event);

    protected:
        ge_common_struct::npc_move_type m_walking_mode;
        int m_frame_counter=0;
        int m_walking_pace=24;
    private:
};

#endif // CNPCGAMEOBJECT_H
