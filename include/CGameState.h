#ifndef CGAMESTATE_H
#define CGAMESTATE_H

#include <string>
#include "CGameContext.h"
#include "common_struct.h"
#include "CGameData.h"

class CGameState
{
    public:
        CGameState();
        CGameState(CGameContext* context);
        virtual ~CGameState();
        virtual void Init()=0;
        virtual void Cleanup()=0;
        virtual void Pause()=0;
        virtual void Resume()=0;
        virtual void HandleEvent(ge_common_struct::input_event event)=0;
        virtual void Update()=0;
        virtual void Draw()=0;
        virtual int GetStateValue()=0;
        virtual void PrepareData()=0;
        void SetGameData(CGameData* gamedata){m_game_data=gamedata;};
    protected:
         CGameContext* m_context;
         int m_state_value;
         CGameData* m_game_data;
    private:

};

#endif // CGAMESTATE_H
