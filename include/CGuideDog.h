#ifndef CGUIDEDOG_H
#define CGUIDEDOG_H

#include <CHandler.h>
template<class GameObject,class EventType>
class CGuideDog: public CHandler
{
public:
    //类GameObject的带有EventType入参的函数指针
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
         //通过指针进行类型转换，但不传指针防止大量的new操作
          EventType* type=static_cast<EventType*>(event);
         (m_object->*m_handle)(*type);
    };


protected:

    GameObject* m_object;
    Handle m_handle;

private:
};

#endif // CGUIDEDOG_H
