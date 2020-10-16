#ifndef CORTHOTILESTATE_H
#define CORTHOTILESTATE_H

#include <CGameState.h>
#include "CSdlGameContext.h"
#include "CRPGGameData.h"
#include "CGameScene.h"
#include "Str_Utilities.h"

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
        int m_movex=0;
        int m_movey=0;
        int m_move_speed=5;
        std::string m_current_action="downward";
        std::string m_current_tileset;
        int m_map_width;
        int m_map_height;
        int m_tile_width;
        int m_tile_height;
        int m_player_x=516;
        int m_player_y=484;
        int m_player_layer=1;
        bool m_player_onstair=false;
        bool m_player_collide=false;
        CGameScene m_game_scene;
        void LoadScene();
        bool CheckCollision(int x,int y,int width,int height,int layer_level);
        bool CheckCollisionByGrid(int gridx,int gridy,int layer_base);


};

#endif // CORTHOTILESTATE_H
