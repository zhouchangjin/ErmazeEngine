#include "CGameEngine.h"
CGameEngine::CGameEngine()
{
}

CGameEngine::~CGameEngine()
{
    m_game_context->ClearUp();
    delete m_game_context;
    m_game_context=NULL;
    for(int i=1;i<=2;i++){
       delete m_states[i];
    }
}

void CGameEngine::Init(){
    LoadSetting();

    m_game_context=new CSdlGameContext();
    //m_game_context->Init(1024,768);
    m_game_context->Init(m_game_setting.GetWindowWidth(),m_game_setting.GetWindowHeight());

    m_current_state=new CMenuState(m_game_context);
    m_current_state->Init();
    m_current_state->PrepareData();

    m_states.insert(std::pair<int,CGameState*>(1,m_current_state));

    COrthoTileState *ortho_tile_state=new COrthoTileState(m_game_context);

    ortho_tile_state->Init();
    ortho_tile_state->SetGameData(m_gamedata);
    ortho_tile_state->PrepareData();

    m_states.insert(std::pair<int,CGameState*>(2,ortho_tile_state));


}

void CGameEngine::Draw(){
    m_current_state->Draw();
    int change_state=m_current_state->GetStateValue();
    if(change_state>0){
        m_current_state=m_states[change_state];
        m_current_state->Resume();
    }else if(change_state==-1){
        m_running=false;
    }

}

void CGameEngine::HandleEvent(){
    ge_common_struct::game_event event=m_game_context->EventCatch();

    if(event==ge_common_struct::QUIT){
        m_running=false;
    }else{
        m_current_state->HandleEvent(event);
    }

}

void CGameEngine::Update(){

}

void CGameEngine::LoadSetting(){
    m_gamedata=new CRPGGameData();
    xmlutils::MyXMLDoc doc=xmlutils::LoadXML(m_setting_file);
    int width=doc.GetIntAttribute("/ermaze/settings/window/@width");
    int height=doc.GetIntAttribute("/ermaze/settings/window/@height");
    m_game_setting.SetWidthHeight(width,height);

    CRPGGameData* gamedata=(CRPGGameData*)m_gamedata;
    gamedata->ParseGameDataByXMLDoc(&doc);

}
