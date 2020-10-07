#ifndef CORTHOTILESTATE_H
#define CORTHOTILESTATE_H

#include <CGameState.h>
#include "CSdlGameContext.h"
#include "CRPGGameData.h"
#include "CGameScene.h"

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
        void PrepareData();
        int GetStateValue();

    protected:

    private:
        CSpriteSheet* m_sprite_sheet;
        CSprite* m_protagnist;
        int m_scale=2;
        int m_step=0;
        std::string m_current_action="downward";

        std::string m_current_tileset;
        int m_map_cell_width;
        int m_map_cell_height;
        int m_tile_width;
        int m_tile_height;
        int m_map_start_x=0;
        int m_map_start_y=0;
        std::vector<int> currentMapData;
        void LoadScene();

};

#endif // CORTHOTILESTATE_H
