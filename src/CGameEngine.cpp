#include "CGameEngine.h"
CGameEngine::CGameEngine()
{
}

CGameEngine::~CGameEngine()
{
    //TODO hard coding
    m_game_context->ClearUp();
    delete m_game_context;
    m_game_context=nullptr;
    for(int i=1;i<=2;i++){
       delete m_states[i];
    }
    delete m_gamedata;
    m_gamedata=nullptr;
}

void CGameEngine::Init(){

    CSimpleGameDB* db=new CSimpleGameDB();
    CServiceLocator::Register(CServiceLocator::DATABASE,db);

    m_game_context=new CSdlGameContext();
    CSdlGameContext* p_context=(CSdlGameContext*)m_game_context;
    p_context->RegisterKey('j',ge_common_struct::key_event_type::KEY_CONFIRM);
    p_context->RegisterKey('k',ge_common_struct::key_event_type::KEY_CANCLE);
    p_context->RegisterKey('w',ge_common_struct::key_event_type::KEY_UP);
    p_context->RegisterKey('s',ge_common_struct::key_event_type::KEY_DOWN);
    p_context->RegisterKey('a',ge_common_struct::key_event_type::KEY_LEFT);
    p_context->RegisterKey('d',ge_common_struct::key_event_type::KEY_RIGHT);
    LoadSetting();
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
    Delay();
    m_current_state->Draw();
    int change_state=m_current_state->GetStateValue();
    if(change_state>0){
        m_current_state=m_states[change_state];
        m_current_state->Resume();
    }else if(change_state==-1){
        m_running=false;
    }
    SetFrameTime();
}

void CGameEngine::HandleEvent(){
    ge_common_struct::input_event event=m_game_context->EventCatch();
    if(event.get_top_event().event==ge_common_struct::QUIT){
        m_running=false;
    }else{
        m_current_state->HandleEvent(event);
    }

}

void CGameEngine::Update(){

    if(m_running){
        m_current_state->Update();
    }

}


int CGameEngine::FrameDiff(){
   return m_game_context->GetTicks()-m_frametime;
}

void CGameEngine::Delay(){
    int min_frametime=m_game_setting.GetMinFrameTime();
    int diff=FrameDiff();
    if(m_cap_frame && diff<min_frametime){
        unsigned int time= min_frametime-(unsigned int)diff;
        //GE_LOG("%u==%u=%u=\n",time,min_frametime,diff);
        m_game_context->DelayTime(time);
    }
}

void CGameEngine::SetFrameTime(){
    m_frametime=m_game_context->GetTicks();
}

void CGameEngine::LoadSetting(){
    m_gamedata=new CRPGGameData();
    xmlutils::MyXMLDoc doc=xmlutils::LoadXML(m_setting_file);
    int width=doc.GetIntAttribute("/ermaze/settings/window/@width");
    int height=doc.GetIntAttribute("/ermaze/settings/window/@height");
    int fps=atoi(doc.GetStr("/ermaze/settings/framerate").c_str());
    m_game_setting.SetWidthHeight(width,height);
    m_game_setting.SetFramePerSecond(fps);
    CRPGGameData* gamedata=(CRPGGameData*)m_gamedata;
    gamedata->ParseGameDataByXMLDoc(&doc);

}
