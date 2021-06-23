#ifndef CPARTICLEPOOL_H
#define CPARTICLEPOOL_H

#include <CParticle.h>
//图形绘制是单线程的，粒子的控制可能会出现（很少）多线程的情况，没有互斥机制
class CParticlePool
{
    public:
        CParticlePool();
        CParticlePool(int max_size);
        virtual ~CParticlePool();
        CParticle* GetOneIdleParitcle();
        void AddToIdlePool(CParticle* particle);
        int GetParicleCnt()const{return m_particle_pool.size();};
    protected:
        int m_max_pool_size=100;
        std::vector<CParticle*> m_particle_pool;
        std::vector<CParticle*> m_idle_particle_pool;
    private:
};

#endif // CPARTICLEPOOL_H
