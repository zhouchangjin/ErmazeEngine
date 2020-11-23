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
        void SetDialogTree(ge_common_struct::dialog_tree_node* node){m_dialog=node;};
        ge_common_struct::dialog_tree_node* GetDialogTree(){return m_dialog;};

    protected:
        ge_common_struct::npc_move_type m_walking_mode;
        int m_frame_counter=0;
        int m_walking_pace=24;
        ge_common_struct::dialog_tree_node* m_dialog=nullptr;
    private:
};

#endif // CNPCGAMEOBJECT_H
