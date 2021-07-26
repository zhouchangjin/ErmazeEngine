#ifndef CANIMATIONMANAGER_H
#define CANIMATIONMANAGER_H

#include <common_struct.h>
#include <CAnimationItem.h>
#include <CParticleSystem.h>
#include <CSpriteDB.h>
#include <CProjectileEmitter.h>

class CAnimationManager
{
    public:
        CAnimationManager();
        virtual ~CAnimationManager();
        void Init();
        void Update();
        void Draw();
        void AddAnimateItem(CAnimationItem item);
        void SetGameContext(CGameContext* context){m_context=context;};
    protected:
        CGameContext* m_context;
        CParticleSystem m_particle_system;
        uint32_t m_frame=0;
        std::vector<CAnimationItem> m_animation_list;
        CSpriteDB* m_sprite_db=nullptr;
        CImageDB* m_image_db=nullptr;

        void CreateProjectile(CAnimationItem item);
    private:
};

#endif // CANIMATIONMANAGER_H
