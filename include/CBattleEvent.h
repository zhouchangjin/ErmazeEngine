#ifndef CBATTLEEVENT_H
#define CBATTLEEVENT_H

#include <CGameEvent.h>


class CBattleEvent : public CGameEvent
{
    public:
        enum action_type{
            DYING,
            DIED
        };
        /** Default constructor */
        CBattleEvent();
        /** Default destructor */
        virtual ~CBattleEvent();

    protected:
        action_type m_action_type=action_type::DYING;
    private:
};

#endif // CBATTLEEVENT_H
