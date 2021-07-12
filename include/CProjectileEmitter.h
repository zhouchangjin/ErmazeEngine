#ifndef CPROJECTILEEMITTER_H
#define CPROJECTILEEMITTER_H

#include <IEmitter.h>
#include <CProjectile.h>
class CProjectileEmitter :public IEmitter
{
    public:
        CProjectileEmitter();
        virtual ~CProjectileEmitter();
        void Init();
        void Update();
        bool IsDead();
        std::vector<CParticle*> GetParticles();
        void SetProjectile(CProjectile projectile);

    protected:
        CProjectile m_projectile;
    private:
};

#endif // CPROJECTILEEMITTER_H
