#ifndef CANIMATIONMANAGER_H
#define CANIMATIONMANAGER_H

#include <common_struct.h>
#include <AnimationItem.h>
#include <CParticleSystem.h>
#include <CSpriteDB.h>

class CAnimationManager
{
    public:
        CAnimationManager();
        virtual ~CAnimationManager();
        void Init();
        void Update();
        void Draw();
        void AddAnimateItem(AnimationItem item);
        void SetGameContext(CGameContext* context){m_context=context;};
    protected:
        CGameContext* m_context;
        CParticleSystem m_particle_system;
        uint32_t m_frame=0;
        std::vector<AnimationItem> m_animation_list;
        CSpriteDB* m_sprite_db=nullptr;
    private:
};

#endif // CANIMATIONMANAGER_H
