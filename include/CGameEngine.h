#ifndef CGAMEENGINE_H
#define CGAMEENGINE_H

#include <string>
#include <vector>
#include <map>

#include "CGameSettings.h"
#include "CGameState.h"
#include "CGameContext.h"
#include "CMenuState.h"
#include "COrthoTileState.h"
#include "CSdlGameContext.h"
#include "CRPGGameData.h"

class CGameEngine
{

    public:
        CGameEngine();
        virtual ~CGameEngine();
        uint32_t GetMinFrametime(){return m_min_frametime;};
        void Init();
        void HandleEvent();
        void Update();
        bool Running(){return m_running;};
        void Quit(){m_running=false;};
        void Draw();
        CGameSettings GetGameSetting(){return m_game_setting;};
        void ChangeState(CGameState* state);
    protected:
        void LoadSetting();

    private:
            std::string m_setting_file="game.xml";
            CGameData* m_gamedata;
            int m_fps=24;
            uint32_t m_min_frametime=1000/m_fps; //Ã¿Ö¡ºÁÃëÊý
            bool m_running=true;                 //ÓÎÏ·¹Ø±Õ×´Ì¬
            CGameSettings m_game_setting;
            CGameState* m_current_state;
            CGameContext* m_game_context;
            std::map<int,CGameState*> m_states;
};

#endif // CGAMEENGINE_H
