#ifndef CGUIDEDOG_H
#define CGUIDEDOG_H

#include <CHandler.h>
template<class GameObject,class EventType>
class CGuideDog: public CHandler
{
public:
    //��GameObject�Ĵ���EventType��εĺ���ָ��
    typedef void (GameObject::*Handle)(EventType);

    CGuideDog(GameObject* object,Handle handle)
    {
        m_object=object;
        m_handle=handle;
    };
    ~CGuideDog(){
    };
    void Call(CGameEvent* event){
          bool flag=event->IsInstanceEvent();
          if(flag){
            void* target=event->GetTarget();
            if((void*)m_object!=target){
                return;
            }
          }
         //ͨ��ָ���������ת����������ָ���ֹ������new����
          EventType* type=static_cast<EventType*>(event);
         (m_object->*m_handle)(*type);
    };


protected:

    GameObject* m_object;
    Handle m_handle;

private:
};

#endif // CGUIDEDOG_H
