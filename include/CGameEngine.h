#ifndef CGAMEENGINE_H
#define CGAMEENGINE_H

#include <string>
#include <vector>
#include <map>

#include <CRandomGenerator.h>
#include <CDatabaseLoader.h>
#include <CSpriteDB.h>

#include "CServiceLocator.h"
#include "CGameSettings.h"
#include "CGameState.h"
#include "CGameContext.h"
#include "CMenuState.h"
#include "COrthoTileState.h"
#include "CSideTurnBaseBattleState.h"
#include "CFailSceneState.h"
#include "CSdlGameContext.h"
#include "CRPGGameData.h"
#include "CSimpleGameDB.h"
#include "CGameDataChunkFactory.h"

class CGameEngine
{

    public:
        CGameEngine();
        virtual ~CGameEngine();
        void Init();
        void HandleEvent();
        void Update();
        bool Running(){return m_running;};
        void Quit(){m_running=false;};
        void Draw();
        CGameSettings GetGameSetting(){return m_game_setting;};
        void ChangeState(CGameState* state);
    protected:
        void LoadDatabase(CGameDatabase* db);
        void LoadObjectList(std::string file_path,ge_fileutil::chunk& chunk,CGameDatabase* db);
        void LoadSetting();
        void SetFrameTime();
        int FrameDiff();
        void Delay();
    private:
            std::string m_setting_file="game.xml";
            CGameData* m_gamedata;
            CDatabaseLoader m_dbloader;
            bool m_cap_frame=true; //限制帧数
            bool m_running=true;   //游戏关闭状态
            CGameSettings m_game_setting;
            CGameState* m_current_state;
            CGameContext* m_game_context;
            std::map<int,CGameState*> m_states;
            uint32_t m_frametime=0;                //当前时间
};

#endif // CGAMEENGINE_H
