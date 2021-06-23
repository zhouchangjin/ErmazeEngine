#ifndef CPARTICLESYSTEM_H
#define CPARTICLESYSTEM_H

#include <CImageDB.h>
#include <Render_Util.h>
#include <CBaseParticleEmitter.h>
#include <CServiceLocator.h>
class CParticleSystem
{
    public:
        CParticleSystem();
        CParticleSystem(CGameContext* context){m_context=context;};
        virtual ~CParticleSystem();
        void Init();
        void Update();
        void Draw();
        void SetGameContext(CGameContext* context){m_context=context;};
        void AddEmmiter(CBaseParticleEmitter* particle_emiter);
        CParticlePool* GetParticlePool(){return &m_particle_pool;};
    protected:

        std::vector<CBaseParticleEmitter*> m_particle_emitters;
        CParticlePool m_particle_pool;
        CGameContext* m_context;
        CImageDB* m_image_db=nullptr;
        std::string m_particle_sheet_name="particles";
    private:
};

#endif // CPARTICLESYSTEM_H
