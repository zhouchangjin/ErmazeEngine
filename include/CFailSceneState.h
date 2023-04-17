#ifndef CFAILSCENESTATE_H
#define CFAILSCENESTATE_H

#include <CGameState.h>
#include <CUIManager.h>

class CFailSceneState : public CGameState
{
    public:
        /** Default constructor */
        CFailSceneState();
        /** Default destructor */
        virtual ~CFailSceneState();
        CFailSceneState(CGameContext * context);
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

        CUIManager m_ui_manager;

    private:
};

#endif // CFAILSCENESTATE_H
