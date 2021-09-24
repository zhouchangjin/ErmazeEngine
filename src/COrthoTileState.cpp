#include "COrthoTileState.h"

COrthoTileState::COrthoTileState()
{
}

COrthoTileState::COrthoTileState(CGameContext* context):CGameState(context)
{
    m_ui_system.SetContext(context);
    m_ui_system.LoadDatabase();
    m_ui_system.LoadUI();
    m_ui_system.LoadActions();
}

COrthoTileState::~COrthoTileState()
{
}

void COrthoTileState::Init()
{
    this->m_state_value=0;
    m_event_manager.EventSubscribe(&m_ui_system,CGameUISystem::ProcessInput);

}

void COrthoTileState::Cleanup()
{
}

void COrthoTileState::Draw()
{
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    if(m_fade_out || m_fade_in )
    {
        if(m_alpha_value>255)
        {
            m_alpha_value=255;
        }
        if(m_alpha_value<0)
        {
            m_alpha_value=0;
        }
        sdlutil2::FillRect(m_context,fullWindow,0,0,0,m_alpha_value);
        FadeInOrOut();
    }
    else
    {
        if(m_scene_loading)
        {

        }
        else
        {


            sdlutil2::FillRect(m_context,fullWindow,0,0,0);
            int camera_x=m_game_scene.GetCamera2DX();
            int camera_y=m_game_scene.GetCamera2DY();

            int cnt=m_game_scene.GetLayerCnt();
            //TODO 优化空白图片不要画，覆盖的不要画。
            for(int i=0; i<cnt; i++)
            {
                sdlutil2::RenderSceneLayer(m_context,m_game_scene,i,camera_x,camera_y,fullWindow,m_scale);
                for(int j=m_player.size()-1; j>=0; j--)
                {
                    CSpriteGameObject* p=m_player[j];
                    int layer=p->GetShowLayer();
                    if(i==layer)
                    {
                        sdlutil2::RenderGameObject(m_context,p,fullWindow,camera_x,camera_y,m_scale);
                    }
                }
                int objs_cnt=m_game_scene.GetNpcCnt();
                for(int o=0; o<objs_cnt; o++)
                {
                    CNPCGameObject* npc=m_game_scene.GetNpc(o);
                    int layer=npc->GetShowLayer();
                    if(i==layer)
                    {
                        sdlutil2::RenderGameObject(m_context,npc,fullWindow,camera_x,camera_y,m_scale);
                    }
                }
            }


        }
    }
    m_ui_system.Draw();
    sdlutil2::RenderPresent(m_context);
}

int COrthoTileState::GetStateValue()
{
    return this->m_state_value;
}

void COrthoTileState::HandleEvent(ge_common_struct::input_event event)
{
    if(!m_key_enable)
    {
        return;
    }

    if(event.get_top_event().key!=ge_common_struct::key_event_type::NO_EVENT)
    {
        LoadSubState();
        CInputEvent inputevent;
        inputevent.SetInputEvent(event);
        inputevent.SetCurrentSubState(m_sub_state);
        m_event_manager.EventPublish(inputevent);
    }
    if(event.get_top_event().key==ge_common_struct::key_event_type::KEY_ESC)
    {
        this->m_state_value=1;
    }
}

void COrthoTileState::Pause()
{

}

void COrthoTileState::Resume()
{
    this->m_state_value=0;


}

void COrthoTileState::Update()
{
    int npc=m_game_scene.GetNpcCnt();
    for(int i=0; i<npc; i++)
    {
        m_game_scene.GetNpc(i)->Move();
    }

    /*Update 方法里处理更新数据，一些复杂的事件也在这里进行判断
    放在前面判断会相对不合理。由于是单一引擎，不需要事件注册的机制
    只有游戏的主角对象会监听事件，而且是强制的，
    所以没必要实现专门的事件监听
    */
    size_t player_cnt=m_player.size();
    if(player_cnt>0)
    {
        for(size_t j=0; j<player_cnt; j++)
        {
            //TODO Event Manager
            UpdateLadder(m_player[j]);
        }
        CPCSpriteGameObject* player=m_player[0];
        if(player->IsMoving())
        {
            bool transfer=CheckTransfer(player);
            if(transfer)
            {
                return;
            }
            bool collision=CheckCollision(player);
            if(collision)
            {
                player->StopMoving();
            }
            else
            {
                bool collision_with_object=CheckCollisionObject(player);
                if(collision_with_object)
                {
                    GE_LOG("Collide with sprites\n");
                    player->StopMoving();
                }

            }
            player->MoveUpdate();
        }
        else if(player->IsActive())
        {
            player->ComsumeAction();
            CNPCGameObject* tnpc=CheckInteract(player);
            if(tnpc!=nullptr)
            {
                CActionEvent talkevent;
                talkevent.SetAction(CActionEvent::action_type::TALK
                                    ,player->GetCurrentOrientation());
                talkevent.SetTarget((void*)tnpc);
                m_event_manager.EventPublish(talkevent);
                if(tnpc->GetDialogTree()){
                   m_ui_system.SetDialogTree(tnpc->GetDialogTree());
                   m_ui_system.ShowDialog();
                }

            }
        }
    }

    for(int i=0; i<npc; i++)
    {
        CNPCGameObject* npc=m_game_scene.GetNpc(i);
        bool collision=CheckCollision(npc);

        if(!collision && player_cnt>0)
        {
            CSpriteGameObject* player=m_player[0];
            collision=npc->CheckCollision(*player);
        }
        if(collision)
        {
            npc->StopMoving();
        }
        npc->MoveUpdate();
    }

    int rand_num=rand()%100;
    if(rand_num==1){
        GE_LOG("Encounter!!                \n");
        CGameDatabase* db=m_ui_system.GetDatabase();
        db->AddObjectToList("battle","e1");
        db->AddObjectToList("battle","e1");
        m_state_value=3;
    }
}

void COrthoTileState::PrepareData()
{
    LoadPlayer();
    LoadScene();
    LoadUI();
}

void COrthoTileState::LoadUI(){
    CRPGGameData* gamedata=(CRPGGameData*)m_game_data;
    m_ui_system.SetDialogStyle(gamedata->GetDialogStyle());
}

void COrthoTileState::LoadPlayer()
{
    if(m_player.size()==0)
    {

        CRPGGameData* gamedata=(CRPGGameData*)m_game_data;
        int cnt=gamedata->GetPlayerOneCnt();
        for(int i=0; i<cnt; i++)
        {
            CSprite* p=gamedata->GetPlayer(i);
            CPCSpriteGameObject* controllable_player=new CPCSpriteGameObject(p);
            m_player.push_back(controllable_player);
            if(i>0)
            {
                m_player[i-1]->BindPal(controllable_player);
            }
            else
            {
                m_event_manager.EventSubscribe(controllable_player,CPCSpriteGameObject::ProcessInput);
            }
        }
    }
}


bool COrthoTileState::CheckCollisionByGrid(int gridx,int gridy,int level)
{

    ge_common_struct::LAYER_IDX layer=m_game_scene.GetTileLayer(level)->GetTiles();
    int idx=layer[gridy][gridx];
    if(idx==0)
    {
        if(level>0)
        {
            ge_common_struct::LAYER_IDX layer_lower=m_game_scene
                                                    .GetTileLayer(level-1)
                                                    ->GetTiles();
            int idx_lower=layer_lower[gridy][gridx];
            bool isladder=m_game_scene.GetSwitchTileBool(idx_lower-1);
            if(isladder)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            if(level+1<m_game_scene.GetLayerCnt())
            {
                ge_common_struct::LAYER_IDX layer_upper=m_game_scene
                                                        .GetTileLayer(level+1)
                                                        ->GetTiles();
                int idx_upper=layer_upper[gridy][gridx];
                bool uppcol=m_game_scene.GetTileCollideBool(idx_upper-1);
                if(!uppcol)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return true;
            }

        }
    }
    else
    {
        bool result=m_game_scene.GetTileCollideBool(idx-1);
        if(result)
        {
            if(level>0)
            {
                ge_common_struct::LAYER_IDX layer_lower=m_game_scene
                                                        .GetTileLayer(level-1)
                                                        ->GetTiles();
                int idx_lower=layer_lower[gridy][gridx];
                bool isladder=m_game_scene.GetSwitchTileBool(idx_lower-1);
                if(isladder)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }
        else
        {
            if(level+1==m_game_scene.GetLayerCnt())
            {
                return false;
            }
            ge_common_struct::LAYER_IDX layer_upper=m_game_scene.GetTileLayer(level+1)->GetTiles();
            int idx_upper=layer_upper[gridy][gridx];
            if(idx_upper==0)
            {
                return false;
            }
            else
            {
                result=m_game_scene.GetTileCollideBool(idx_upper-1);
                return result;
            }
        }
    }
}

bool COrthoTileState::CheckCollision(int x,int y,int width,
                                     int height,int level,
                                     int move_x,int move_y)
{
    int swidth=m_game_scene.GetMapWidth();
    int sheight=m_game_scene.GetMapHeight();
    int gwidth=m_game_scene.GetTileWidth();
    int gheight=m_game_scene.GetTileHeight();

    int gridLeftX=x/gwidth;
    int gridFootY=(y+height-1)/gheight;
    int gridRightX=(x+width-1)/gwidth;
    int gridHeadY=y/gheight;
    if(gridFootY>=sheight || gridHeadY<0 || gridLeftX<0 || gridRightX>=swidth)
    {
        //超出边界
        return true;
    }
    if(level>=m_game_scene.GetLayerCnt())
    {
        return true;
    }
    if( (move_x<0 || move_y<0)
            && CheckCollisionByGrid(gridLeftX,gridHeadY,level))
    {
        return true;
    }
    else if( (move_x>0 || move_y<0)
             && CheckCollisionByGrid(gridRightX,gridHeadY,level))
    {
        return true;
    }
    else if( (move_x<0 || move_y>0)
             && CheckCollisionByGrid(gridLeftX,gridFootY,level))
    {
        return true;
    }
    else if( (move_x>0 || move_y>0)
             && CheckCollisionByGrid(gridRightX,gridFootY,level))
    {
        return true;
    }

    return false;


}

bool COrthoTileState::CheckCollision(CSpriteGameObject* player)
{

    int movex=player->GetMoveX();
    int movey=player->GetMoveY();
    int player_x=player->GetX();
    int player_y=player->GetY();
    int layer=player->GetLayer();
    int width=player->GetSprite()->GetSpriteSheet()->GetSpriteWidth();
    int height=player->GetSprite()->GetSpriteSheet()->GetSpriteHeight();
    int spd=player->GetMoveSpeed();
    if(movex>0)
    {
        player_x+=spd;
    }
    else if(movex<0)
    {
        player_x-=spd;
    }
    if(movey<0)
    {
        player_y-=spd;
    }
    else if(movey>0)
    {
        player_y+=spd;
    }
    return CheckCollision(player_x,player_y,width,height,layer,
                          movex,movey);
}

int COrthoTileState::GetGridIdx(int gridx,int gridy,int level)
{
    if(gridx<0 || gridy<0 || gridx>=m_game_scene.GetMapWidth()
            || gridy>=m_game_scene.GetMapHeight())
    {
        return 0;
    }
    if(level>=m_game_scene.GetLayerCnt())
    {
        return 0;
    }
    ge_common_struct::LAYER_IDX layer=m_game_scene.GetTileLayer(level)
                                      ->GetTiles();
    int idx=layer[gridy][gridx];
    return idx;
}

ge_common_struct::grid_type COrthoTileState::GetGridType(int gridx,
        int gridy,int level)
{
    ge_common_struct::grid_type type=ge_common_struct::grid_type::NORMAL;
    int idx=GetGridIdx(gridx,gridy,level);
    if(idx==0)
    {
        type=ge_common_struct::grid_type::EMPTY;
    }
    else
    {
        bool collide=m_game_scene.GetTileCollideBool(idx-1);
        if(collide)
        {
            type=ge_common_struct::grid_type::COLLISION;
        }
        else
        {
            bool ladder=m_game_scene.GetSwitchTileBool(idx-1);
            if(ladder)
            {
                type=ge_common_struct::grid_type::LADDER;
            }
        }
    }
    return type;
}

void COrthoTileState::UpdateLadder(CSpriteGameObject* object)
{

    // 不论对象是上下移动还是左右移动逻辑都相同
    // 如果onstair=false  脚在梯子中间，那么onstair=true，图层显示stair上面一层
    // 如果onstair=true   脚离开梯子，那么如果所属图层只有某一层有数据，则进入那一层
    int layer=object->GetLayer();
    int movex=object->GetMoveX();
    int movey=object->GetMoveY();
    int x=object->GetX();
    int y=object->GetY();
    int height=object->GetSprite()->GetSpriteSheet()->GetSpriteHeight();
    int width=object->GetSprite()->GetSpriteSheet()->GetSpriteWidth();
    int gheight=m_game_scene.GetTileHeight();
    int gwidth=m_game_scene.GetTileWidth();
    int spd=object->GetMoveSpeed();
    if(movex>0)
    {
        x+=spd;
    }
    else if(movex<0)
    {
        x-=spd;
    }
    if(movey>0)
    {
        y+=spd;
    }
    else if(movey<0)
    {
        y-=spd;
    }
    int gridFootY=(y+height-1)/gheight;
    int gridCenterX=(x+width/2)/gwidth;

    ge_common_struct::grid_type uptile=GetGridType(gridCenterX,gridFootY,layer);
    ge_common_struct::grid_type downtile=ge_common_struct::EMPTY;
    bool onladder=false;
    if(layer>0)
    {
        downtile=GetGridType(gridCenterX,gridFootY,layer-1);
    }
    if(uptile==ge_common_struct::grid_type::LADDER
            || downtile==ge_common_struct::grid_type::LADDER)
    {
        onladder=true;
    }
    if(onladder)
    {
        object->SetOnStair(true);
        if(uptile==ge_common_struct::grid_type::LADDER)
        {
            object->UpdateShowLayer(layer+1);
        }
    }
    else
    {
        if(object->IsOnStair())
        {
            object->SetOnStair(false);
            if(uptile==ge_common_struct::EMPTY)
            {
                //level need to be changed;
                if(downtile!=ge_common_struct::EMPTY)
                {
                    object->UpdateLayer(layer-1);
                }
                else
                {
                    object->UpdateLayer(layer+1);
                }
            }
        }
    }
}

bool COrthoTileState::CheckTransfer(CSpriteGameObject* object)
{

    int x=object->GetX();
    int y=object->GetY();
    int w=object->GetSprite()->GetSpriteSheet()->GetSpriteWidth();
    int h=object->GetSprite()->GetSpriteSheet()->GetSpriteHeight();

    int mid_x=x+w/2;
    int mid_y=y+h/2;
    bool findTransferEvent=false;
    int findArea=-1;
    for(findArea=0; findArea<m_game_scene.GetTransferAreaCnt(); findArea++)
    {
        CTransferArea area=m_game_scene.GetTransferArea(findArea);
        int area_minx=area.GetX();
        int area_maxx=area_minx+area.GetWidth();
        int area_miny=area.GetY();
        int area_maxy=area_miny+area.GetHeight();
        if(mid_x>area_minx && mid_x<area_maxx && mid_y>area_miny && mid_y<area_maxy)
        {
            findTransferEvent=true;
            break;
        }
    }

    if(findTransferEvent)
    {
        CTransferArea area=m_game_scene.GetTransferArea(findArea);

        ExitScene();
        CRPGGameData* gamedata=(CRPGGameData*)m_game_data;
        gamedata->SetCurrentScene(area.GetScene());
        ge_common_struct::start_point2d point2d;
        point2d.x=area.GetMapX();
        point2d.y=area.GetMapY();
        point2d.layer=area.GetMapLayer();
        if(area.GetDirection()==ge_common_struct::action_type::MOVE_UP)
        {
            point2d.direction="upward";
        }
        else if(area.GetDirection()==ge_common_struct::action_type::MOVE_DOWN)
        {
            point2d.direction="downward";
        }
        else if(area.GetDirection()==ge_common_struct::action_type::MOVE_LEFT)
        {
            point2d.direction="leftward";
        }
        else if(area.GetDirection()==ge_common_struct::action_type::MOVE_RIGHT)
        {
            point2d.direction="rightward";
        }
        point2d.start_scene=area.GetScene();
        gamedata->SetStartPoint(point2d);
        for(size_t i=0; i<m_player.size(); i++)
        {
            m_player[i]->ClearActionLog();
            m_player[i]->ClearMove();
        }
        LoadScene();


    }

    return findTransferEvent;




}

void COrthoTileState::FadeInOrOut()
{
    if(m_fade_out)
    {
        m_alpha_value+=20;
    }
    else if(m_fade_in)
    {
        m_alpha_value-=20;
    }
    if(m_alpha_value>=255)
    {
        m_fade_out=false;
    }
    if(m_alpha_value<=0)
    {
        m_fade_in=false;
    }
}

void COrthoTileState::ExitScene()
{
    if(!m_fade_out)
    {
        m_alpha_value=0;
        m_fade_out=true;
    }
}

void COrthoTileState::LoadScene()
{
    m_key_enable=false;
    m_scene_loading=true;
    m_game_scene.ClearScene();
    CActionEvent event;
    m_event_manager.ClearSubscriber(event);
    CRPGGameData* gamedata=(CRPGGameData*)m_game_data;
    CSceneData scene=gamedata->GetCurrentScene();
    GE_LOG("Loading Scene %s\n",scene.GetTileMapPath().c_str());
    std::string scene_file="./scenes/"+scene.GetTileMapPath()+".tmx";
    std::string object_file="./scenes/"+scene.GetTileMapPath()+".objs";
    xmlutils::MyXMLDoc doc =xmlutils::LoadXML(scene_file);
    xmlutils::MyXMLDoc obj_doc=xmlutils::LoadXML(object_file);
    xmlutils::MyXMLNode docmap=doc.GetNode("/map");

    int map_width=docmap.IntAttribute("width");
    int map_height=docmap.IntAttribute("height");
    int tile_width=docmap.IntAttribute("tilewidth");
    int tile_height=docmap.IntAttribute("tileheight");

    m_game_scene.SetTileWidth(tile_width);
    m_game_scene.SetTileHeight(tile_height);
    m_game_scene.SetMapWidth(map_width);
    m_game_scene.SetMapHeight(map_height);

    std::string tileset_file_name=doc.GetStrAttribute("/map/tileset/@source");

    xmlutils::MyXMLNode node=doc.GetNode("/map/layer");

    for(; node; node=node.NextSlibing("layer"))
    {
        CTileLayer* tiled_layer=new CTileLayer;
        std::string csvdata=node.Child("data").ValueStr();
        std::vector<std::string> lines=ge_str_utilities::SplitStr(csvdata,'\n');
        for(size_t s=0; s<lines.size(); s++)
        {
            std::string l=ge_str_utilities::TrimStr(lines[s]);
            if(l.size()>0)
            {
                ge_common_struct::ROW_IDX row=ge_str_utilities::SplitStrToIntArray(l,',');
                tiled_layer->AddRow(row);
            }

        }

        m_game_scene.AddLayer(tiled_layer);
    }

    xmlutils::MyXMLNode obj_groupnode=doc.GetNode("/map/objectgroup");

    for(; obj_groupnode; obj_groupnode=obj_groupnode.NextSlibing("objectgroup"))
    {
        xmlutils::MyXMLNode obj_node=obj_groupnode.Child("object");

        for(; obj_node; obj_node=obj_node.NextSlibing("object"))
        {
            std::string obj_type=obj_node.StrAttribute("type");
            if(obj_type.compare("transfer")==0)
            {
                int x=obj_node.IntAttribute("x");
                int y=obj_node.IntAttribute("y");
                int width=obj_node.IntAttribute("width");
                int height=obj_node.IntAttribute("height");
                int transfer_x=0;
                int transfer_y=0;
                int direction=1;
                std::string scene="";
                int layer=1;

                xmlutils::MyXMLNode properties=obj_node.Child("properties");
                if(properties)
                {
                    xmlutils::MyXMLNode pnode=properties.Child("property");
                    for(; pnode; pnode=pnode.NextSlibing("property"))
                    {
                        std::string name=pnode.StrAttribute("name");
                        if(name.compare("scene")==0)
                        {
                            scene=pnode.StrAttribute("value");
                        }
                        else if(name.compare("x")==0)
                        {
                            transfer_x=pnode.IntAttribute("value");
                        }
                        else if(name.compare("y")==0)
                        {
                            transfer_y=pnode.IntAttribute("value");
                        }
                        else if(name.compare("layer")==0)
                        {
                            layer=pnode.IntAttribute("value");
                        }
                        else if(name.compare("direction")==0)
                        {
                            direction=pnode.IntAttribute("value");
                        }
                    }
                }
                CTransferArea area;
                area.SetX(x);
                area.SetY(y);
                area.SetMapX(transfer_x);
                area.SetMapY(transfer_y);
                area.SetScene(scene);
                area.SetDirection((ge_common_struct::action_type)direction);
                area.SetMapLayer(layer);
                area.SetWidth(width);
                area.SetHeight(height);
                m_game_scene.AddTransferArea(area);
            }

        }

    }

    std::string tileset_file_path="./scenes/"+ tileset_file_name;

    xmlutils::MyXMLDoc tilesetdoc=xmlutils::LoadXML(tileset_file_path);
    std::string tileset_image_file=tilesetdoc.GetStrAttribute("/tileset/image/@source");

    std::string current_tileset="./scenes/"+tileset_image_file;

    xmlutils::MyXMLNode tilenode=tilesetdoc.GetNode("/tileset/tile");

    for(; tilenode; tilenode=tilenode.NextSlibing("tile"))
    {
        int id= tilenode.IntAttribute("id");
        xmlutils::MyXMLNode property=tilenode.Child("properties").Child("property");
        for(; property; property=property.NextSlibing("property"))
        {

            std::string value=property.StrAttribute("value");
            if(strcmp(property.StrAttribute("name").c_str()
                      ,scene.GetTileCollideProp().c_str())==0)
            {

                if(strcmp("true",value.c_str())==0)
                {
                    m_game_scene.AddCollideTile(id);
                }
            }
            else if(strcmp(property.StrAttribute("name").c_str(),
                           scene.GetLayerSwitchProp().c_str())==0)
            {
                if(strcmp("true",value.c_str())==0)
                {
                    m_game_scene.AddSwitchTile(id);
                }
            }
        }
    }

    void* map_texture=sdlutil2::LoadPngTexture(current_tileset,m_context);
    m_game_scene.SetTexture(map_texture);

    //放置对象
    xmlutils::MyXMLNode cha_node=obj_doc.GetNode("/scene/characters/character");
    xmlutils::MyXMLNode obj_node=obj_doc.GetNode("/scene/objects/object");

    for(; cha_node; cha_node=cha_node.NextSlibing("character"))
    {
        std::string sheet_id=cha_node.StrAttribute("sheet_id");
        std::string id=cha_node.StrAttribute("id");
        CSpriteSheet* sheet=gamedata->GetSpriteSheet(sheet_id);
        m_game_scene.AddSprite(id,sheet);
        //////////////////
        xmlutils::MyXMLNode actionsnode=cha_node.Child("actions");
        if(actionsnode)
        {
            xmlutils::MyXMLNode action_node=actionsnode.Child("action");
            for(; action_node; action_node=action_node.NextSlibing("action"))
            {
                std::string action_name=action_node.StrAttribute("name");
                std::string actiontile=action_node.StrAttribute("ids");
                std::vector<int> ids=ge_str_utilities::
                                     SplitStrToIntArray(actiontile,',');
                m_game_scene.AddSpriteAction(id,action_name,ids);

            }
        }
    }

    for(; obj_node; obj_node=obj_node.NextSlibing("object"))
    {
        std::string otype=obj_node.StrAttribute("type");
        if(otype.compare("character")==0)
        {
            int x=obj_node.IntAttribute("x");
            int y=obj_node.IntAttribute("y");
            int layer=obj_node.IntAttribute("layer");
            int direction=obj_node.IntAttribute("direction");
            std::string sprite_id=obj_node.StrAttribute("refid");
            CNPCGameObject* npc=m_game_scene.CreateNpc(sprite_id,x,y
                                ,layer,direction);
            m_event_manager.EventSubscribe(npc,CNPCGameObject::OnAction);
            xmlutils::MyXMLNode w_node=obj_node.Child("walkingmode");
            xmlutils::MyXMLNode dialog_node=obj_node.Child("dialogtree");
            if(w_node)
            {
                std::string mode=w_node.StrAttribute("type");
                if(mode.compare("halt")==0)
                {
                    npc->SetWalkingMode(ge_common_struct::npc_move_type::NPC_HALT);
                }
                else if(mode.compare("random")==0)
                {
                    npc->SetWalkingMode(ge_common_struct::npc_move_type::NPC_RANDOM);
                }
                else
                {
                    npc->SetWalkingMode(ge_common_struct::npc_move_type::NPC_STILL);
                }
            }

            if(dialog_node)
            {
                xmlutils::MyXMLNode tree_root=dialog_node.Child("node");
                ge_common_struct::dialog_tree_node* tree_node=ge_fileutil
                        ::parse_dialog_tree(tree_root);
                npc->SetDialogTree(tree_node);
            }

        }
    }
    GE_LOG("Found %d Characters \n",m_game_scene.GetNpcCnt());
    //放置角色
    if(m_player.size()>0)
    {
        GE_LOG("Repositioning player to %d,%d\n",
               gamedata->GetStartX(),gamedata->GetStartY());
        m_player[0]->BindCamera(m_game_scene.GetCameraPointer());
        for(size_t i=0; i<m_player.size(); i++)
        {
            m_player[i]->UpdateXY(gamedata->GetStartX(),gamedata->GetStartY());
            m_player[i]->UpdateDirection(gamedata->GetStartDirection());
            m_player[i]->UpdateLayer(gamedata->GetStartLayer());
        }
    }

    m_key_enable=true;
    m_scene_loading=false;
}


bool COrthoTileState::CheckCollisionObject(CSpriteGameObject* object)
{
    bool collision=false;
    int npc_cnt=m_game_scene.GetNpcCnt();
    for(int i=0; i<npc_cnt; i++)
    {
        CNPCGameObject* npc=m_game_scene.GetNpc(i);
        collision=object->CheckCollision(*npc);
        if(collision)
        {
            break;
        }
    }
    return collision;
}

void COrthoTileState::LoadSubState(){
    if(m_ui_system.EventLock()){
        m_sub_state=substate::UI_STATE;
    }else{
        m_sub_state=substate::TILE_STATE;
    }
}

CNPCGameObject* COrthoTileState::CheckInteract(CSpriteGameObject* object)
{
    CNPCGameObject* npc_capture=nullptr;
    std::string action_name=object->GetCurrentAction();
    int ray_x=0;
    int ray_y=0;
    if(action_name.compare("upward")==0)
    {
        ray_y=-1;
    }
    else if(action_name.compare("downward")==0)
    {
        ray_y=1;
    }
    else if(action_name.compare("leftward")==0)
    {
        ray_x=-1;
    }
    else if(action_name.compare("rightward")==0)
    {
        ray_x=1;
    }
    else
    {
        return nullptr;
    }
    int npccnt=m_game_scene.GetNpcCnt();
    for(int i=0; i<npccnt; i++)
    {
        CNPCGameObject* npc=m_game_scene.GetNpc(i);
        int x=npc->GetX();
        int y=npc->GetY();
        ge_common_struct::ge_point coor_npc=m_game_scene.GetGridCoor(x,y);
        int px=object->GetX();
        int py=object->GetY();
        ge_common_struct::ge_point coor=m_game_scene.GetGridCoor(px,py);
        if(coor.x+ray_x==coor_npc.x && coor.y+ray_y==coor_npc.y)
        {
            GE_LOG("talking to npc\n");
            npc_capture=npc;
            break;
        }
    }
    return npc_capture;

}
