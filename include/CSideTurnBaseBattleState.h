#ifndef CSIDETURNBASEBATTLESTATE_H
#define CSIDETURNBASEBATTLESTATE_H

#include <EngineHelper.h>
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
#include <CRPGEnemyData.h>
#include <CRPGPlayerData.h>

class CSideTurnBaseBattleState: public CGameState
{
    public:
        static const uint32_t TIMECHECK=UINT_MAX+100;
        enum substate{
            BATTLE_STATE,
            COMMAND_INIT_STATE,
            COMMAND_STATE,
            BATTLE_INIT_STATE,
            BATTLE_COMMAND_PREPARE_STATE,
            ENDING_STATE,
            EXIT_STATE,
            FAIL_STATE
        };
        enum battletype{
            PARTY_TURN_BASED,
            INDI_TURN_BASED,
            ATB_TURN_BASED
        };
        enum object_type{
            PLAYER,
            ENEMY,
            NEUTRAL,
            OTHER
        };

        enum event_type{
            ENEMY_DIED,
            PLAYER_DIED,
            ACCOUNT_END
        };

        struct timer_event{
            uint32_t frame;
            int object_id;
            int object_no;
            object_type object_type;
            event_type event_type;
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

       std::vector<CSpriteGameObject> m_players;  //need reset
       std::vector<CSpriteGameObject> m_enemies;  //need reset

       std::vector<CRPGPlayerData> m_player_data;  //reset
       std::vector<CRPGEnemyData> m_enemy_data;   // reset

       std::vector<CRPGEnemyData> m_defeat_enemies;  //reset

       //状态标志
       substate m_substate;                        // reset
       //菜单指示名称
       std::string m_menu_pointer="point_right";

       uint32_t m_frame;                        //need reset
       uint32_t m_last_timer;                   //need reset

       uint32_t m_batterprepare_time=10;
       float m_playerinit_position=1.0f;
       int m_player_accerleration=5;

       uint32_t m_current_command_player;        //need reset

       std::vector<ge_common_struct::command_item> m_seq_command_list; //reset

       //std::map<int,int> m_frame_command_map;

       int m_player_scale=2;
       int m_enemy_scale=2;

       ge_common_struct::ge_rect m_enemy_rect;
       ge_common_struct::ge_rect m_player_rect;

       std::vector<timer_event> m_timer_event;        //reset

       std::vector<std::string> m_log_text;           //reset


    private:
        void LoadComponents();
        void LoadUIDef();
        void ResetBattle();
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
        void Accounting();
        bool IsPast(uint32_t time);

        void InitMenu(ge_common_struct::input_event& event);

        void AddCommand(ge_common_struct::command_item item);
        void LogCommand(std::vector<ge_common_struct::menu_command>& command);
        void TranslateCommand(std::vector<ge_common_struct::menu_command>& command);
        void SortCommand();
        void AddEnemyCommand();


        object_type GetObjectType(int obj_id);

        void FlashEnemy(int enemy_no);
        void MoveForwardPlayer(int player_no);
        void HitEnemy(int player_no,int enemy_no,int with_obj=-1);
        void HitPlayer(int enemy_no,int player_no);
        void DisappearEnemy(int enemy_no);

        void CheckEvent();
        void ProcessTimerEvent(timer_event event);

        int CalcDamage(int player_no,int enemy_no,int with_object);
        int CalcEnemyDamage(int ene_no,int play_no);


        void GetAlivePlayer(std::vector<int>& alive_players);
        int ChangeTargetofEnemy();

        void FirstFunctionalPlayer();
        void NextFunctionalPlayer();
        void PreviousCommandPlayer();
};

#endif // CSIDETURNBASEBATTLESTATE_H
