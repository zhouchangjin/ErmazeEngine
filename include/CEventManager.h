#ifndef CEVENTMANAGER_H
#define CEVENTMANAGER_H

#include<vector>
#include<map>
#include<string>
#include<CGuideDog.h>

typedef std::vector<CHandler*> Handlers;


class CEventManager
{
public:
    CEventManager(){};
    ~CEventManager(){};

    template<class EventType>
    void EventPublish(EventType eventType)
    {
        std::string typen=typeid(EventType).name();
        Handlers * handlers = m_guidedogs[typen];
        if(handlers==nullptr)
        {
            return ;
        }

        for(size_t i=0; i<(*handlers).size(); i++)
        {
            (*handlers)[i]->Exec(&eventType);
        }
    }

    template<class GameObject,class EventType>
    void EventSubscribe(GameObject* object
                        ,void (GameObject::*Handle)(EventType))
    {
        std::string typen=typeid(EventType).name();
        Handlers * handlers = m_guidedogs[typen];
        if(handlers==nullptr)
        {
            handlers=new Handlers();
            m_guidedogs[typen]=handlers;
        }
        handlers->push_back(new CGuideDog<GameObject,EventType>(object,Handle));
    }

    template<class EventType>
    void ClearSubscriber(EventType eventType){
        std::string typen=typeid(EventType).name();
        Handlers * handlers = m_guidedogs[typen];
        if(handlers!=nullptr){
           handlers->clear();
        }

    }


protected:
    std::map<std::string, Handlers*> m_guidedogs;
private:
};

#endif // CEVENTMANAGER_H
