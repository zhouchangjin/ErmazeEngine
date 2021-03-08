#include "CGameEngine.h"
CGameEngine::CGameEngine()
{
}

CGameEngine::~CGameEngine()
{
    m_game_context->ClearUp();
    delete m_game_context;
    m_game_context=nullptr;

    std::map<int,CGameState*>::iterator it;

    for(it=m_states.begin();it!=m_states.end();it++){
       delete it->second;
    }
    m_states.clear();
    delete m_gamedata;
    m_gamedata=nullptr;
}

void CGameEngine::Init(){

    CSimpleGameDB* db=new CSimpleGameDB();
    CImageDB* image_db=new CImageDB();
    CServiceLocator::Register(CServiceLocator::DATABASE,db);
    CServiceLocator::Register(CServiceLocator::TEXTURE_DB,image_db);
    image_db->Initialize();
    //demo db
    //db->SetIntData("tmp",9); //demo
    db->CreateList("players");
    db->CreateList("tanks");
    db->AddPropToType("player","icon","icon",CGameDatabase::DataType::ICON_ID);
    db->AddPropToType("tank","icon","icon",CGameDatabase::DataType::ICON_ID);
    int id1=db->StoreObject("pro_1","wolf","player");
    int id2=db->StoreObject("pro_2","fish","player");
    int id3=db->StoreObject("pro_3","bear","player");
    int id4=db->StoreObject("pro_4","dog","player");

    int tid1=db->StoreObject("t_1","tank1","tank");
    int tid2=db->StoreObject("t_2","tank2","tank");
    int tid3=db->StoreObject("t_3","tank3","tank");


    db->SetObjectText(id1,"icon","point_right");
    db->SetObjectText(id2,"icon","point_right");
    db->SetObjectText(id3,"icon","point_right");
    db->SetObjectText(id4,"icon","point_right");

    db->SetObjectText(tid1,"icon","point_right");
    db->SetObjectText(tid2,"icon","point_right");
    db->SetObjectText(tid3,"icon","point_right");

    db->AddObjectToList("players",id1);
    db->AddObjectToList("players",id2);
    db->AddObjectToList("players",id3);
    db->AddObjectToList("players",id4);

    db->AddObjectToList("tanks",tid1);
    db->AddObjectToList("tanks",tid2);
    db->AddObjectToList("tanks",tid3);

    //db->RemoveObjectFromList("players",id4);


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

    CSideTurnBaseBattleState * btt_state=new CSideTurnBaseBattleState(m_game_context);
    btt_state->Init();
    btt_state->SetGameData(m_gamedata);
    btt_state->PrepareData();

    m_states[3]=btt_state;

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
    if(event.get_top_event().key==ge_common_struct::QUIT){
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
