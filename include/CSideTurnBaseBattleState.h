#ifndef CSIDETURNBASEBATTLESTATE_H
#define CSIDETURNBASEBATTLESTATE_H

#include <XML_Utilities.h>
#include <GameFileLoader.h>
#include <CGameState.h>
#include <CUIManager.h>
#include <CEventManager.h>
#include <CParticleSystem.h>
#include <CBaseParticleEmitter.h>
#include <CProjectileEmitter.h>
#include <CSpriteDB.h>
#include <CServiceLocator.h>
#include <CAnimationManager.h>

class CSideTurnBaseBattleState: public CGameState
{
    public:
        enum substate{
            BATTLE_STATE,
            COMMAND_INIT_STATE,
            COMMAND_STATE,
            BATTLE_INIT_STATE,
            BATTLE_COMMAND_PREPARE_STATE
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

       CAnimationManager m_animation_manager;

       //精灵库
       CSpriteDB* m_sprite_db=nullptr;
       //数据库
       CGameDatabase* m_database=nullptr;

       std::vector<CSpriteGameObject> m_players;
       std::vector<CSpriteGameObject> m_enemies;

       //状态标志
       substate m_substate=substate::BATTLE_INIT_STATE;
       //菜单指示名称
       std::string m_menu_pointer="point_right";

       uint32_t m_frame=0;
       uint32_t m_last_timer=0;

       uint32_t m_current_command_player=0;
       uint32_t m_current_animation_item=-1;

       std::vector<ge_common_struct::command_item> m_seq_command_list;

       int m_player_scale=3;
       int m_enemy_scale=2;

       ge_common_struct::ge_rect m_enemy_rect;
       ge_common_struct::ge_rect m_player_rect;

    private:
        void LoadComponents();
        void LoadUIDef();
        void LoadSprites();

        void UpdateEnemy();
        void UpdatePlayer();
        void UpdateHUD();
        void UpdateBattleField();
        void DrawBattleField();
        void DrawEnemy();
        void DrawPlayer();
        void DrawHUD();
        void DrawMenu();

        void ProcessBattle();
        void UpdateBattle();

        void InitMenu(ge_common_struct::input_event& event);

        void AddCommand(ge_common_struct::command_item item);
        void LogCommand(std::vector<ge_common_struct::menu_command>& command);
        void TranslateCommand(std::vector<ge_common_struct::menu_command>& command);

};

#endif // CSIDETURNBASEBATTLESTATE_H
