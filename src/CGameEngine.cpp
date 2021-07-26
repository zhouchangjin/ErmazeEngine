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
    m_game_context=new CSdlGameContext();

    CServiceLocator::Register(CServiceLocator::DATABASE,db);
    CServiceLocator::Register(CServiceLocator::TEXTURE_DB,image_db);
    CServiceLocator::Register(CServiceLocator::SPRITE_DB,sprite_db);

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

    m_states[3]=btt_state;

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

void CGameEngine::LoadDatabase(CGameDatabase* db)
{
    std::string filepath="./data/metadata.xml";
    xmlutils::MyXMLDoc doc=xmlutils::LoadXML(filepath);
    xmlutils::MyXMLNode xml_node=doc.GetNode("/filedef/chunk");
    ge_fileutil::chunk chunk;
    ge_fileutil::parse_chunk(xml_node,chunk);
    std::string playerdata="./data/player.dat";
    std::string tankdata="./data/tank.dat";
    std::string enemydata="./data/enemy.dat";

    db->CreateList("players");
    db->CreateList("tanks");
    db->CreateList("battle");

    ge_fileutil::CBinaryFileReader filereader(playerdata);
    filereader.ReOpen();
    CGameDataChunkFactory factory;
    ge_fileutil::parse_chunk_file_bydef(filereader,chunk,&factory);
    filereader.Close();

    ge_fileutil::CBinaryFileReader tankreader(tankdata);
    tankreader.ReOpen();
    CGameDataChunkFactory tfactory;
    ge_fileutil::parse_chunk_file_bydef(tankreader,chunk,&tfactory);
    tankreader.Close();

    ge_fileutil::CBinaryFileReader enereader(enemydata);
    enereader.ReOpen();
    CGameDataChunkFactory efactory;
    ge_fileutil::parse_chunk_file_bydef(enereader,chunk,&efactory);

    //factory.PrintAll();
    //tfactory.PrintAll();

    CDatabaseLoader loader;
    loader.SetGameDatabase(db);
    loader.LoadObjects(&factory);
    loader.LoadObjects(&tfactory);
    loader.LoadObjects(&efactory);

    db->AddObjectToList("players","p1");
    db->AddObjectToList("players","p2");
    db->AddObjectToList("players","p3");
    db->AddObjectToList("players","p4");
    db->AddObjectToList("tanks","t1");
    db->AddObjectToList("tanks","t2");
    db->AddObjectToList("tanks","t3");
    db->AddObjectToList("battle","e1");
    db->AddObjectToList("battle","e2");

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
