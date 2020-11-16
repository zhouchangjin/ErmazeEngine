#ifndef CGAMEEVENT_H
#define CGAMEEVENT_H
#include "common_struct.h"

class CGameEvent
{
    public:
        CGameEvent();
        virtual ~CGameEvent();
        int GetEventX()const{return m_event_x;};
        int GetEventY()const{return m_event_y;};
        ge_common_struct::game_event GetEventType()const{return m_event_type;};
        void SetEventX(int x){m_event_x=x;};
        void SetEventY(int y){m_event_y=y;};
        void SetEventType(ge_common_struct::game_event type){m_event_type=type;};

    protected:
        ge_common_struct::game_event m_event_type;
        int m_event_x;
        int m_event_y;

    private:
};

#endif // CGAMEEVENT_H
