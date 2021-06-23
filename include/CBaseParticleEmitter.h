#ifndef CBASEPARTICLEEMITTER_H
#define CBASEPARTICLEEMITTER_H

#include <CParticlePool.h>

class CBaseParticleEmitter
{
    public:
        CBaseParticleEmitter(CParticlePool *pool);
        virtual ~CBaseParticleEmitter();
        void Init();
        void Update();
        std::vector<CParticle*> GetParticles()const{return m_particles;};

    protected:

        CParticlePool* m_particle_pool;
        std::vector<CParticle*> m_particles;
        int m_particle_cnt=30;
        int m_particle_regenerate=1;
        uint32_t m_max_particle_cnt=60;
        int m_particle_max_life=20;
        int m_emitter_life=4000;
        int m_pos_x=250;
        int m_pos_y=250;

        void InitParticle(CParticle* particle);
        CParticle* CreateParticle();

    private:
};

#endif // CBASEPARTICLEEMITTER_H
