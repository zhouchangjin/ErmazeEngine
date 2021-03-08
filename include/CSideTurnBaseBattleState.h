#ifndef CSIDETURNBASEBATTLESTATE_H
#define CSIDETURNBASEBATTLESTATE_H

#include <XML_Utilities.h>
#include <GameFileLoader.h>
#include <Render_Util.h>
#include <CServiceLocator.h>
#include <CGameState.h>
#include <CImageDB.h>
#include <CGameDatabase.h>

class CSideTurnBaseBattleState: public CGameState
{
    public:
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

        std::map<std::string,ge_common_struct::dom_node*> m_panels;
        std::map<std::string,ge_common_struct::dom_node*> m_menus;


        CGameDatabase* m_database=nullptr;
        CImageDB* m_imagedb=nullptr;

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
