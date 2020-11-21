#ifndef CACTIONEVENT_H
#define CACTIONEVENT_H

#include <CGameEvent.h>


class CActionEvent : public CGameEvent
{
    public:
        enum action_type{
            TALK,
            OPEN,
            OTHER
        };
        CActionEvent();
        virtual ~CActionEvent();
        action_type GetActionType()const{return m_action_type;};
        int GetActionDirection()const{return m_action_direction;};
        void SetAction(action_type type,int direction){m_action_type=type;m_action_direction=direction;};

    protected:
        action_type m_action_type=action_type::TALK;
        int m_action_direction;
    private:
};

#endif // CACTIONEVENT_H
