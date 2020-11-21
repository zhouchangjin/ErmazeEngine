#ifndef CGAMEEVENT_H
#define CGAMEEVENT_H
#include "common_struct.h"

class CGameEvent
{
    public:
        CGameEvent();
        virtual ~CGameEvent();
        void SetTarget(void *target){m_target=target;};
        void* GetTarget(){return m_target;};
        void SetInstanceEvent(){m_instance_event=true;};
        bool IsInstanceEvent()const{return m_instance_event;};
    protected:
        void* m_target;
        bool m_instance_event=false;

    private:
};

#endif // CGAMEEVENT_H
