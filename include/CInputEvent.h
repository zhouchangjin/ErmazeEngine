#ifndef CINPUTEVENT_H
#define CINPUTEVENT_H

#include <CGameEvent.h>


class CInputEvent : public CGameEvent
{
    public:
        CInputEvent();
        virtual ~CInputEvent();
        ge_common_struct::input_event GetInput()const{return m_input;};
        void SetInputEvent(ge_common_struct::input_event input){m_input=input;};
        int GetCurrentSubState()const{return m_current_sub_state;};
        void SetCurrentSubState(int state_value){m_current_sub_state=state_value;};
    protected:
        ge_common_struct::input_event m_input;
        int m_current_sub_state=0;

    private:
};

#endif // CINPUTEVENT_H
