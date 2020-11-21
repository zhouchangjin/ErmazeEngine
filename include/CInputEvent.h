#ifndef CINPUTEVENT_H
#define CINPUTEVENT_H

#include <CGameEvent.h>


class CInputEvent : public CGameEvent
{
    public:
        CInputEvent();
        virtual ~CInputEvent();
        ge_common_struct::input_event GetEventType()const{return m_event_type;};
        void SetEventType(ge_common_struct::input_event type){m_event_type=type;};
        int GetCurrentSubState()const{return m_current_sub_state;};
        void SetCurrentSubState(int state_value){m_current_sub_state=state_value;};
    protected:
        ge_common_struct::input_event m_event_type;
        int m_current_sub_state=0;

    private:
};

#endif // CINPUTEVENT_H
