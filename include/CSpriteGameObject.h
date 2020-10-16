#ifndef CSPRITEGAMEOBJECT_H
#define CSPRITEGAMEOBJECT_H

#include <CGameObject.h>
#include <CSprite.h>

class CSpriteGameObject  : public CGameObject
{
    public:
        CSpriteGameObject();
        virtual ~CSpriteGameObject();

    protected:

    private:

        CSprite m_sprite;
};

#endif // CSPRITEGAMEOBJECT_H
