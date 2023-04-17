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

    for(it=m_states.begin(); it!=m_states.end(); it++)
    {
        delete it->second;
    }
    m_states.clear();
    delete m_gamedata;
    m_gamedata=nullptr;
}

void CGameEngine::Init()
{

    CSimpleGameDB* db=new CSimpleGameDB();
    CImageDB* image_db=new CImageDB();
    CSpriteDB* sprite_db=new CSpriteDB();
    CRandomGenerator* generator=new CRandomGenerator();

    m_game_context=new CSdlGameContext();

    CServiceLocator::Register(CServiceLocator::DATABASE,db);
    CServiceLocator::Register(CServiceLocator::TEXTURE_DB,image_db);
    CServiceLocator::Register(CServiceLocator::SPRITE_DB,sprite_db);
    CServiceLocator::Register(CServiceLocator::RANDOM_ENGINE,generator);

    image_db->Initialize();
    sprite_db->Initialize();

    LoadSetting();
    LoadDatabase(db);


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

    m_states[3]=btt_state; //map的插入方式

    CFailSceneState *fail_state=new CFailSceneState(m_game_context);
    fail_state->Init();
    fail_state->SetGameData(m_gamedata);
    fail_state->PrepareData();

    m_states.insert(std::pair<int,CGameState*>(4,fail_state));

}

void CGameEngine::Draw()
{
    Delay();
    m_current_state->Draw();
    int change_state=m_current_state->GetStateValue();
    if(change_state>0)
    {
        m_current_state=m_states[change_state];
        m_current_state->Resume();
    }
    else if(change_state==-1)
    {
        m_running=false;
    }
    SetFrameTime();
}

void CGameEngine::HandleEvent()
{
    ge_common_struct::input_event event=m_game_context->EventCatch();
    if(event.get_top_event().key==ge_common_struct::QUIT)
    {
        m_running=false;
    }
    else
    {
        m_current_state->HandleEvent(event);
    }

}

void CGameEngine::Update()
{

    if(m_running)
    {
        m_current_state->Update();
    }

}


int CGameEngine::FrameDiff()
{
    return m_game_context->GetTicks()-m_frametime;
}

void CGameEngine::Delay()
{
    int min_frametime=m_game_setting.GetMinFrameTime();
    int diff=FrameDiff();
    if(m_cap_frame && diff<min_frametime)
    {
        unsigned int time= min_frametime-(unsigned int)diff;
        //GE_LOG("%u==%u=%u=\n",time,min_frametime,diff);
        m_game_context->DelayTime(time);
    }
}

void CGameEngine::SetFrameTime()
{
    m_frametime=m_game_context->GetTicks();
}

void CGameEngine::LoadObjectList(std::string file_path,
                                 ge_fileutil::chunk& chunk,CGameDatabase* db){

    ge_fileutil::CBinaryFileReader filereader(file_path);
    filereader.ReOpen();
    CGameDataChunkFactory factory;
    ge_fileutil::parse_chunk_file_bydef(filereader,chunk,&factory);
    filereader.Close();
    m_dbloader.LoadObjects(&factory);

}

void CGameEngine::LoadDatabase(CGameDatabase* db)
{

    m_dbloader.SetGameDatabase(db);

    std::string filepath="./data/metadata.xml";
    xmlutils::MyXMLDoc doc=xmlutils::LoadXML(filepath);
    xmlutils::MyXMLNode xml_node=doc.GetNode("/filedef/chunk");
    ge_fileutil::chunk chunk;
    ge_fileutil::parse_chunk(xml_node,chunk);

    std::string playerdata="./data/player.dat";
    std::string tankdata="./data/tank.dat";
    std::string enemydata="./data/enemy.dat";
    std::string weapondata="./data/weapon.dat";
    std::vector<std::string> filelist;
    filelist.push_back(playerdata);
    filelist.push_back(tankdata);
    filelist.push_back(enemydata);
    filelist.push_back(weapondata);

    for(size_t i=0;i<filelist.size();i++){
        LoadObjectList(filelist[i],chunk,db);
    }

    db->CreateList("players");
    db->CreateList("tanks");
    db->CreateList("battle");
    db->AddObjectToList("players","p1");
    db->AddObjectToList("players","p2");
    db->AddObjectToList("players","p3");
    db->AddObjectToList("players","p4");
    db->AddObjectToList("tanks","t1");
    db->AddObjectToList("tanks","t2");
    db->AddObjectToList("tanks","t3");
    //db->AddObjectToList("battle","e1");
    //db->AddObjectToList("battle","e1");

    db->SetTextData("ui_ind","point_right");

    int p1Id=db->GetObjectId("p1");
    int p2Id=db->GetObjectId("p2");
    int p3Id=db->GetObjectId("p3");
    int p4Id=db->GetObjectId("p4");

    db->AddPropToType("equipment","lefthand","左手",CGameDatabase::DataType::OBJECT_ID);

    int e1id=db->StoreObject("w1","装备","equipment");
    int e2id=db->StoreObject("w2","装备","equipment");
    int e3id=db->StoreObject("w3","装备","equipment");
    int e4id=db->StoreObject("w4","装备","equipment");

    db->AddPropToType("player","equipment","装备",CGameDatabase::DataType::OBJECT_ID);
    db->SetObjectData(p1Id,"equipment",e1id);
    db->SetObjectData(p2Id,"equipment",e2id);
    db->SetObjectData(p3Id,"equipment",e3id);
    db->SetObjectData(p4Id,"equipment",e4id);
    int weaponid=db->GetObjectId("knife");
    std::string s=db->GetObjectText(weaponid,"animation");
    //GE_LOG("weapon id is %d  %s\n",weaponid,s.c_str());
    db->SetObjectData(e1id,"lefthand",weaponid);
    db->SetObjectData(e2id,"lefthand",-1);
    db->SetObjectData(e3id,"lefthand",-1);
    db->SetObjectData(e4id,"lefthand",-1);
}

void CGameEngine::LoadSetting()
{
    m_gamedata=new CRPGGameData();
    xmlutils::MyXMLDoc doc=xmlutils::LoadXML(m_setting_file);
    int width=doc.GetIntAttribute("/ermaze/settings/window/@width");
    int height=doc.GetIntAttribute("/ermaze/settings/window/@height");
    int fps=atoi(doc.GetStr("/ermaze/settings/framerate").c_str());
    xmlutils::MyXMLNode node=doc.GetNode("/ermaze/settings/keysetting");
    xmlutils::MyXMLNode keynode=node.Child("keybinding");
    for(;keynode;keynode=keynode.NextSlibing("keybinding")){
        std::string keyname=keynode.StrAttribute("keyname");
        char key=keyname.c_str()[0];
        std::string keytypename=keynode.StrAttribute("event_type");
        ge_common_struct::key_event_type type =ge_common_struct
        ::TranslateEventTypeName(keytypename);
        CSdlGameContext* p_context=(CSdlGameContext*)m_game_context;
        p_context->RegisterKey(key,type);
    }
    m_game_setting.SetWidthHeight(width,height);
    m_game_setting.SetFramePerSecond(fps);
    CRPGGameData* gamedata=(CRPGGameData*)m_gamedata;
    gamedata->ParseGameDataByXMLDoc(&doc);

}
