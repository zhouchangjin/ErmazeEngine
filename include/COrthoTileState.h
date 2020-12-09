#ifndef CORTHOTILESTATE_H
#define CORTHOTILESTATE_H



#include <CGameState.h>
#include <CSceneData.h>
#include <CSdlGameContext.h>
#include <CRPGGameData.h>
#include <C2DGameScene.h>
#include <Str_Utilities.h>
#include <CEventManager.h>
#include <CInputEvent.h>
#include <CPCSpriteGameObject.h>
#include <CGameUISystem.h>
#include <GameFileLoader.h>
class COrthoTileState : public CGameState
{
    public:
        enum substate{
            TILE_STATE,
            UI_STATE
        };
        COrthoTileState();
        COrthoTileState(CGameContext* context);
        virtual ~COrthoTileState();
        void Init();
        void Cleanup();
        void Pause();
        void Resume();
        void HandleEvent(ge_common_struct::input_event event);
        void Update();
        void Draw();
        void PrepareData();
        int GetStateValue();
        void ExitScene();
        void FadeInOrOut();

    protected:

    private:
        bool m_key_enable=true;
        bool m_fade_out=false;
        bool m_fade_in=false;
        bool m_scene_loading=false;
        int m_alpha_value=255;
        int m_scale=4;
        substate m_sub_state=substate::TILE_STATE;

        CEventManager m_event_manager;
        C2DGameScene m_game_scene;
        CGameUISystem m_ui_system;

        std::vector<CPCSpriteGameObject*> m_player;
        std::vector<CPCSpriteGameObject*> m_player_two;

        //private function
        void LoadUI();
        void LoadScene();
        void LoadPlayer();
        void LoadSubState();
        CNPCGameObject* CheckInteract(CSpriteGameObject* object);
        bool CheckCollisionObject(CSpriteGameObject* object);
        bool CheckTransfer(CSpriteGameObject* object);
        bool CheckCollision(CSpriteGameObject* object);
        bool CheckCollision(int x,int y,int width,int height,int layer_level,
                            int move_x,int move_y);
        bool CheckCollisionByGrid(int gridx,int gridy,int layer_base);
        void UpdateLadder(CSpriteGameObject* object);
        int GetGridIdx(int gridx,int gridy,int level);
        ge_common_struct::grid_type GetGridType(int gridx,int gridy,int level);
};

#endif // CORTHOTILESTATE_H
