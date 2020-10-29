#include "COrthoTileState.h"

COrthoTileState::COrthoTileState()
{


}

COrthoTileState::COrthoTileState(CGameContext* context):CGameState(context)
{
    //ctor
}

COrthoTileState::~COrthoTileState()
{
}

void COrthoTileState::Init()
{
    this->m_state_value=0;
}

void COrthoTileState::Cleanup()
{

}

void COrthoTileState::Draw()
{
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
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
    }
    sdlutil2::RenderPresent(m_context);

}

int COrthoTileState::GetStateValue()
{
    return this->m_state_value;
}

void COrthoTileState::HandleEvent(ge_common_struct::game_event event)
{
    CSpriteGameObject* player=nullptr;
    if(m_player.size()>0)
    {
        player=m_player[0];
    }
    if(event==ge_common_struct::KEY_ESC)
    {
        this->m_state_value=1;
    }
    else if(player)
    {
        if(event==ge_common_struct::KEY_UP)
        {
            player->MoveUpward(16);
        }
        else if(event==ge_common_struct::KEY_DOWN)
        {
            player->MoveDownward(16);
        }
        else if(event==ge_common_struct::KEY_LEFT)
        {
            player->MoveLeftward(16);
        }
        else if(event==ge_common_struct::KEY_RIGHT)
        {
            player->MoveRightward(16);
        }

    }
    else
    {

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
        CSpriteGameObject* player=m_player[0];
        if(player->IsMoving())
        {
            bool collision=CheckCollision(player);
            if(collision)
            {
                player->StopMoving();
            }
            else
            {
                player->MoveUpdate();
            }
        }
    }

}

void COrthoTileState::PrepareData()
{
    LoadPlayer();
    LoadScene();
}

void COrthoTileState::LoadPlayer()
{
    CRPGGameData* gamedata=(CRPGGameData*)m_game_data;
    int cnt=gamedata->GetPlayerOneCnt();
    for(int i=0; i<cnt; i++)
    {
        CSprite* p=gamedata->GetPlayer(i);
        CSpriteGameObject* controllable_player=new CSpriteGameObject(p);
        m_player.push_back(controllable_player);
        if(i>0){
            m_player[i-1]->BindPal(controllable_player);
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
                if(!uppcol){
                    return false;
                }else{
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
    if(movex>0){
        player_x+=spd;
    }else if(movex<0){
        player_x-=spd;
    }
    if(movey<0){
        player_y-=spd;
    }else if(movey>0){
        player_y+=spd;
    }
    return CheckCollision(player_x,player_y,width,height,layer,
                          movex,movey);
}

int COrthoTileState::GetGridIdx(int gridx,int gridy,int level)
{
    if(gridx<0 || gridy<0 || gridx>=m_game_scene.GetMapWidth()
       || gridy>=m_game_scene.GetMapHeight()){
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
    if(movex>0){
        x+=spd;
    }else if(movex<0){
        x-=spd;
    }
    if(movey>0){
        y+=spd;
    }else if(movey<0){
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

void COrthoTileState::LoadScene()
{
    m_game_scene.ClearScene();
    CRPGGameData* gamedata=(CRPGGameData*)m_game_data;
    CSceneData scene=gamedata->GetCurrentScene();
    std::string scenefile="./scenes/"+scene.GetTileMapPath()+".tmx";
    xmlutils::MyXMLDoc doc =xmlutils::LoadXML(scenefile);
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
        std::string csvdata=node.Child("data").valueStr();
        std::vector<std::string> lines=ge_str_utilities::Splitstr(csvdata,'\n');
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

    if(m_player.size()>0)
    {
        m_player[0]->BindCamera(m_game_scene.GetCameraPointer());
        m_player[0]->UpdateXY(gamedata->GetStartX(),gamedata->GetStartY());
        m_player[0]->UpdateDirection(gamedata->GetStartDirection());
        m_player[0]->UpdateLayer(gamedata->GetStartLayer());
    }


}
