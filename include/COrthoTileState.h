#ifndef CORTHOTILESTATE_H
#define CORTHOTILESTATE_H

#include <CGameState.h>
#include "CSdlGameContext.h"
#include "CRPGGameData.h"

class COrthoTileState : public CGameState
{
    public:
        COrthoTileState();
        COrthoTileState(CGameContext* context);
        virtual ~COrthoTileState();
        void Init();
        void Cleanup();
        void Pause();
        void Resume();
        void HandleEvent(ge_common_struct::game_event event);
        void Update();
        void Draw();
        int GetStateValue();

    protected:

    private:
        CSpriteSheet* m_sprite_sheet;
        CSprite* m_protagnist;
        int m_scale=2;
        int m_step=0;
        std::string m_current_action="downward";

};

#endif // CORTHOTILESTATE_H
