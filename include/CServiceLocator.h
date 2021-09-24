#ifndef CSERVICELOCATOR_H
#define CSERVICELOCATOR_H
#include<map>

class CServiceLocator
{
    public:
        enum ServiceID{
            DATABASE,
            TEXTURE_DB,
            SPRITE_DB,
            GAME_CONTEXT,
            RANDOM_ENGINE
        };
        template<class ServiceType>
        static ServiceType* GetService(CServiceLocator::ServiceID id){
            ServiceType* pointer=static_cast<ServiceType*>(m_services[id]);
            return pointer;
        }

        template<class ServiceType>
        static void Register(CServiceLocator::ServiceID id,ServiceType* instance){
            m_services[id]=(void*)instance;
        }
    protected:
    private:

        static std::map<CServiceLocator::ServiceID,void*> m_services;
};

#endif // CSERVICELOCATOR_H
