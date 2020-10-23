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
    //dtor
    delete this->m_sprite_sheet;
    m_sprite_sheet=NULL;
    delete this->m_protagnist;
    m_protagnist=NULL;
}

void COrthoTileState::Init()
{
    this->m_sprite_sheet=new CSpriteSheet();
    this->m_state_value=0;
    std::string path=this->m_sprite_sheet->GetSpritePath();

    //��Ϊû��������ע�� ����ʹ��SDLʱ��λ�þͲ��ò���
    void* sprite_texture=sdlutil2::LoadPngTexture(path,m_context);
    m_sprite_sheet->SetTexture(sprite_texture);
    std::vector<int> sequence= {0,1,2,3,4,5,6,7};
    m_protagnist=new CSprite(m_sprite_sheet,sequence);
    m_protagnist->AddAction("downward", {0,1});
    m_protagnist->AddAction("upward", {2,3});
    m_protagnist->AddAction("left", {4,5});
    m_protagnist->AddAction("right", {6,7});
}

void COrthoTileState::Cleanup()
{

}

void COrthoTileState::Draw()
{
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    sdlutil2::FillRect(m_context,fullWindow,0,0,0);
    int centerx=fullWindow.w/2;
    int centery=fullWindow.h/2;


    int cnt=m_game_scene.GetLayerCnt();
    //TODO �Ż��հ�ͼƬ��Ҫ�������ǵĲ�Ҫ����
    for(int i=0; i<cnt; i++)
    {
        sdlutil2::RenderSceneLayer(m_context,m_game_scene,i,m_player_x,m_player_y,fullWindow,m_scale);
        if(i==m_player_layer)
        {
            int idx=m_protagnist->PlayAction(m_current_action,m_step);
            sdlutil2::RenderSprite(m_context,m_protagnist,idx,centerx,centery,m_scale);
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

    if(event==ge_common_struct::KEY_ESC)
    {
        this->m_state_value=1;
    }
    else if(event==ge_common_struct::KEY_UP)
    {
        m_current_action="upward";
        m_movey-=m_move_speed;
    }
    else if(event==ge_common_struct::KEY_DOWN)
    {
        m_current_action="downward";
        m_movey+=m_move_speed;
    }
    else if(event==ge_common_struct::KEY_LEFT)
    {
        m_current_action="left";
        m_movex-=m_move_speed;
    }
    else if(event==ge_common_struct::KEY_RIGHT)
    {
        m_current_action="right";
        m_movex+=m_move_speed;
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

    /*Update �����ﴦ��������ݣ�һЩ���ӵ��¼�Ҳ����������ж�
    ����ǰ���жϻ���Բ����������ǵ�һ���棬����Ҫ�¼�ע��Ļ���
    ֻ����Ϸ�����Ƕ��������¼���������ǿ�Ƶģ�
    ����û��Ҫʵ��ר�ŵ��¼�����
    */
    if(m_movex!=0 || m_movey!=0)
    {
        // ��һ�� �жϾ�������layer��������������ӣ������������һlayer����ײ����һlayer
        m_player_collide=CheckCollision(m_player_x+m_movex,m_player_y+m_movey
                             ,16,16,m_player_layer);
        if(m_player_collide){
            if(m_movey>0){ //��Ϊ������ͼ�����������������£��������������Ȳ����ǣ�����ͼ����Ϸ�Ƚ��٣���
                if(m_player_layer>0){
                    int gridFootY=(m_player_y+16)/m_game_scene.GetTileHeight();
                    int gridX=(m_player_x+8)/m_game_scene.GetTileWidth();
                    int gridHeadY=m_player_y/m_game_scene.GetTileHeight();
                    ge_common_struct::LAYER_IDX layer=m_game_scene.GetTileLayer(m_player_layer-1)->GetTiles();
                    if(m_player_onstair==false){
                        int id=layer[gridFootY][gridX];
                        bool onstair=m_game_scene.GetSwitchTileBool(id-1);
                        if(onstair==true){
                            m_player_onstair=true;
                        }
                    }else{
                        int id=layer[gridHeadY][gridX];
                        bool onstair=m_game_scene.GetSwitchTileBool(id-1);
                        if(onstair==true){
                            m_player_layer--;
                            m_player_onstair=false;
                        }
                    }
                }
            }else if(m_movey<0){
                int gridFootY=(m_player_y+16)/m_game_scene.GetTileHeight();
                int gridX=(m_player_x+8)/m_game_scene.GetTileWidth();
                if(m_player_layer+1<m_game_scene.GetLayerCnt()){
                    ge_common_struct::LAYER_IDX layer=m_game_scene.GetTileLayer(m_player_layer)->GetTiles();
                    int id=layer[gridFootY][gridX];
                    bool onstair=m_game_scene.GetSwitchTileBool(id-1);
                    if(onstair){
                        m_player_layer++;
                    }

                }
            }
        }
    }

    if(m_player_collide && !m_player_onstair)
    {
        m_movex=0;
        m_movey=0;
    }
    else
    {

        if(m_movex!=0)
        {
            m_step++;
            m_player_x=m_player_x+m_movex;
            m_movex=0;
        }
        else if(m_movey!=0)
        {
            m_step++;
            m_player_y=m_player_y+m_movey;
            m_movey=0;
        }


    }




}

void COrthoTileState::PrepareData()
{

    LoadScene();

}

bool COrthoTileState::CheckCollisionByGrid(int gridx,int gridy,int level)
{

    ge_common_struct::LAYER_IDX layer=m_game_scene.GetTileLayer(level)->GetTiles();
    int idx=layer[gridy][gridx];
    if(idx==0)
    {
        return true;
    }
    else
    {
        bool result=m_game_scene.GetTileCollideBool(idx-1);
        if(result)
        {
            return true;
        }
        else
        {
            if(level+1==m_game_scene.GetLayerCnt()){
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
                                     int height,int level)
{
    int swidth=m_game_scene.GetMapWidth();
    int sheight=m_game_scene.GetMapHeight();
    int gwidth=m_game_scene.GetTileWidth();
    int gheight=m_game_scene.GetTileHeight();

    int gridLeftX=(x+m_move_speed)/gwidth;
    int gridFootY=(y+height-m_move_speed)/gheight;
    int gridRightX=(x+width-m_move_speed)/gwidth;
    int gridHeadY=(y+m_move_speed)/gheight;
    if(gridFootY>=sheight || gridHeadY<0 || gridLeftX<0 || gridRightX>=swidth)
    {
        //�����߽�
        return true;
    }

    if( (m_movex<0 || m_movey<0)
            && CheckCollisionByGrid(gridLeftX,gridHeadY,level))
    {
        return true;
    }
    else if( (m_movex>0 || m_movey<0)
             && CheckCollisionByGrid(gridRightX,gridHeadY,level))
    {
        return true;
    }
    else if( (m_movex<0 || m_movey>0)
             && CheckCollisionByGrid(gridLeftX,gridFootY,level))
    {
        return true;
    }
    else if( (m_movex>0 || m_movey>0)
             && CheckCollisionByGrid(gridRightX,gridFootY,level))
    {
        return true;
    }

    return false;


}

void COrthoTileState::LoadScene()
{
    m_game_scene.ClearScene();
    CRPGGameData* gamedata=(CRPGGameData*)m_game_data;
    C2DGameScene scene=gamedata->GetCurrentScene();
    std::string scenefile="./scenes/"+scene.GetTileMapPath()+".tmx";
    xmlutils::MyXMLDoc doc =xmlutils::LoadXML(scenefile);
    xmlutils::MyXMLNode docmap=doc.GetNode("/map");
    m_map_width=docmap.IntAttribute("width");
    m_map_height=docmap.IntAttribute("height");
    m_tile_width=docmap.IntAttribute("tilewidth");
    m_tile_height=docmap.IntAttribute("tileheight");

    m_game_scene.SetTileWidth(m_tile_width);
    m_game_scene.SetTileHeight(m_tile_height);
    m_game_scene.SetMapWidth(m_map_width);
    m_game_scene.SetMapHeight(m_map_height);


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

    m_current_tileset="./scenes/"+tileset_image_file;

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

    void* map_texture=sdlutil2::LoadPngTexture(m_current_tileset,m_context);
    m_game_scene.SetTexture(map_texture);

}
