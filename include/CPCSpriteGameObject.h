#ifndef CPCSPRITEGAMEOBJECT_H
#define CPCSPRITEGAMEOBJECT_H

#include <CSpriteGameObject.h>
#include <IDefaultEventProcess.h>

class CPCSpriteGameObject : public CSpriteGameObject,public IDefaultEventProcess
{
    public:
        CPCSpriteGameObject(CSprite* sprite);
        virtual ~CPCSpriteGameObject();
        void ProcessInput(CInputEvent event);
        bool IsActive();
        void ComsumeAction();
    protected:
        bool m_action_active=false;
    private:
};

#endif // CPCSPRITEGAMEOBJECT_H
