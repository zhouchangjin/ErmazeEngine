#ifndef CSIDETURNBASEBATTLESTATE_H
#define CSIDETURNBASEBATTLESTATE_H

#include <CGameState.h>

class CSideTurnBaseBattleState: public CGameState
{
    public:
        CSideTurnBaseBattleState();
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

    private:
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
