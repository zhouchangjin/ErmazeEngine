#include "CParticlePool.h"

CParticlePool::CParticlePool()
{
    //ctor
}

CParticlePool::CParticlePool(int max_size){

    if(max_size>0){
       m_max_pool_size=max_size;
    }

}

CParticlePool::~CParticlePool()
{
    while(m_idle_particle_pool.size()>0){
        m_idle_particle_pool.pop_back();
    }

    for(size_t i=0;i<m_particle_pool.size();i++){
        CParticle* p=m_particle_pool[i];
        delete p;
        p=nullptr;
        m_particle_pool[i]=nullptr;
    }
    while(m_particle_pool.size()>0){
        m_particle_pool.pop_back();
    }
}

CParticle* CParticlePool::GetOneIdleParitcle()
{
    if(m_idle_particle_pool.size()==0){
        if(m_particle_pool.size()<(uint32_t)m_max_pool_size){
            CParticle* p=new CParticle();
            m_particle_pool.push_back(p);
            return p;
        }else{
            return nullptr;
        }
    }else{
        CParticle* idle=m_idle_particle_pool.back();
        m_idle_particle_pool.pop_back();
        return idle;
    }
}

void CParticlePool::AddToIdlePool(CParticle* idle){
    if(m_idle_particle_pool.size()<(uint32_t)m_max_pool_size){
       m_idle_particle_pool.push_back(idle);
    }
}
