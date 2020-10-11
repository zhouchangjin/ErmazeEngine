#include "COrthoTileState.h"
#include <iostream>
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

    //因为没有用依赖注入 当不使用SDL时该位置就不得不变
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

    for(int i=0;i<cnt;i++){
        sdlutil2::RenderSceneLayer(m_context,m_game_scene,i,m_player_x,m_player_y,fullWindow,2);
    }




    int idx=m_protagnist->PlayAction(m_current_action,m_step);
    sdlutil2::RenderSprite(m_context,m_protagnist,idx,centerx,centery,2);
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
        m_step++;
        m_current_action="upward";
        m_player_y-=5;
    }
    else if(event==ge_common_struct::KEY_DOWN)
    {
        m_step++;
        m_current_action="downward";
        m_player_y+=5;
    }
    else if(event==ge_common_struct::KEY_LEFT)
    {
        m_step++;
        m_current_action="left";
        m_player_x-=5;
    }
    else if(event==ge_common_struct::KEY_RIGHT)
    {
        m_step++;
        m_current_action="right";
        m_player_x+=5;
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

}

void COrthoTileState::PrepareData()
{

    LoadScene();

}

void COrthoTileState::LoadScene()
{
    m_game_scene.ClearScene();
    CRPGGameData* gamedata=(CRPGGameData*)m_game_data;
    CGameScene scene=gamedata->GetCurrentScene();
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
        std::string csvdata=node.Child("data").valueStr();
        std::vector<std::string> lines=ge_str_utilities::Splitstr(csvdata,'\n');
        ge_common_struct::LAYER_IDX layer;
        for(size_t s=0; s<lines.size(); s++)
        {
            std::string l=ge_str_utilities::TrimStr(lines[s]);
            if(l.size()>0)
            {
                ge_common_struct::ROW_IDX row=ge_str_utilities::SplitStrToIntArray(l,',');
                layer.push_back(row);
            }

        }
        m_game_scene.AddLayer(layer);
    }
    std::string tileset_file_path="./scenes/"+ tileset_file_name;

    xmlutils::MyXMLDoc tilesetdoc=xmlutils::LoadXML(tileset_file_path);
    std::string tileset_image_file=tilesetdoc.GetStrAttribute("/tileset/image/@source");

    m_current_tileset="./scenes/"+tileset_image_file;
    //std::cout<<"Image File "<<m_current_tileset<<std::endl;

    void* map_texture=sdlutil2::LoadPngTexture(m_current_tileset,m_context);
    m_game_scene.SetTexture(map_texture);

}
