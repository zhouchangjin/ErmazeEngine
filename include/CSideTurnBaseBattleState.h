#ifndef CSIDETURNBASEBATTLESTATE_H
#define CSIDETURNBASEBATTLESTATE_H

#include <XML_Utilities.h>
#include <GameFileLoader.h>
#include <CGameState.h>
#include <CUIManager.h>
#include <CEventManager.h>


class CSideTurnBaseBattleState: public CGameState
{
    public:
        enum substate{
            BATTLE_STATE,
            COMMAND_INIT_STATE,
            COMMAND_STATE
        };
        CSideTurnBaseBattleState();
        CSideTurnBaseBattleState(CGameContext * context);
        virtual ~CSideTurnBaseBattleState();
        void Init();
        void Cleanup();
        void Pause();
        void Resume();
        void HandleEvent(ge_common_struct::input_event event);
        void Update();
        void Draw();
        int GetStateValue();
        void PrepareData();

    protected:
       //事件管理
       CEventManager m_event_manager;
       //界面管理
       CUIManager m_ui_manager;
       //状态标志
       substate m_substate=substate::COMMAND_INIT_STATE;
       //菜单指示名称
       std::string m_menu_pointer="point_right";
    private:
        void LoadComponents();
        void LoadUIDef();
        void LoadTexture();
        void LoadSprites();
        void LoadAnimation();

        void UpdateEnemy();
        void UpdatePlayer();
        void UpdateHUD();
        void UpdateBattleField();
        void DrawBattleField();
        void DrawEnemy();
        void DrawPlayer();
        void DrawHUD();
        void DrawMenu();

};

#endif // CSIDETURNBASEBATTLESTATE_H
