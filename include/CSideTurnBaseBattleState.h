#ifndef CSIDETURNBASEBATTLESTATE_H
#define CSIDETURNBASEBATTLESTATE_H

#include <XML_Utilities.h>
#include <GameFileLoader.h>
#include <CGameState.h>
#include <CUIManager.h>
#include <CEventManager.h>
#include <CParticleSystem.h>

class CSideTurnBaseBattleState: public CGameState
{
    public:
        enum substate{
            BATTLE_STATE,
            COMMAND_INIT_STATE,
            COMMAND_STATE,
            BATTLE_INIT_STATE
        };
        enum battletype{
            PARTY_TURN_BASED,
            INDI_TURN_BASED,
            ATB_TURN_BASED
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

       //粒子动画系统
       CParticleSystem m_particle_system;

       //状态标志
       substate m_substate=substate::BATTLE_INIT_STATE;
       //菜单指示名称
       std::string m_menu_pointer="point_right";

       uint32_t m_temp_timer=0;
       uint32_t m_last_timer=0;

       int m_current_command_player=0;

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

        void InitMenu(ge_common_struct::input_event& event);

};

#endif // CSIDETURNBASEBATTLESTATE_H
