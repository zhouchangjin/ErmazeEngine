#ifndef CBASEPARTICLEEMITTER_H
#define CBASEPARTICLEEMITTER_H

#include <IEmitter.h>
#include <CParticlePool.h>

class CBaseParticleEmitter :public IEmitter
{
    public:
        CBaseParticleEmitter(CParticlePool *pool);
        virtual ~CBaseParticleEmitter();
        void Init();
        void Update();
        bool IsDead();
        int GetX(){return m_pos_x;};
        int GetY(){return m_pos_y;};
        void UpdateX(int x){m_pos_x=x;};
        void UpdateY(int y){m_pos_y=y;};
        std::vector<CParticle*> GetParticles(){return m_particles;};

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
