#include "CRPGGameData.h"
CRPGGameData::CRPGGameData()
{

}

CRPGGameData::~CRPGGameData()
{
    //delete the pointer;
    std::map<std::string,CSpriteSheet*>::iterator iter;
    for(iter=m_sprite_sheets.begin(); iter!=m_sprite_sheets.end(); iter++)
    {
        CSpriteSheet* pointer=iter->second;
        delete pointer;
    }
}

void CRPGGameData::AddPlayer(std::string id)
{
    m_player_one.push_back(id);
}

int CRPGGameData::GetPlayerOneCnt()
{
    return m_player_one.size();
}

CSprite* CRPGGameData::GetPlayer(int id)
{
    std::string pid=m_player_one[id];
    CSprite *sprite=m_sprites[pid];
    return sprite;
}

void CRPGGameData::ClearPlayer()
{
    m_player_one.clear();
}

void CRPGGameData::AddSprite(std::string id,std::string sheet_id)
{
    CSpriteSheet* sheet=m_sprite_sheets[sheet_id];
    if(sheet)
    {
        CSprite* sprite=new CSprite(sheet);
        m_sprites[id]=sprite;
    }

}

void CRPGGameData::AddAction(std::string id,std::string action_name,
                             std::vector<int> ids)
{
    CSprite* sprite=m_sprites[id];
    sprite->AddAction(action_name,ids);
}

CSceneData CRPGGameData::GetCurrentScene()
{
    return m_scene_list[m_current_scene];
}

void CRPGGameData::SetCurrentScene(std::string scene)
{
    m_current_scene=scene;
}

void CRPGGameData::AddSpriteSheet(std::string id,std::string path,int width,
                                  int height, int col,int row)
{
    CSpriteSheet* sheet=new CSpriteSheet(path,width,height,col,row);
    m_sprite_sheets[id]=sheet;
}

CSpriteSheet* CRPGGameData::GetSpriteSheet(std::string id)
{
    return m_sprite_sheets[id];
}

void CRPGGameData::SetStartPoint(ge_common_struct::start_point2d point)
{
    m_startpoint=point;
}

void CRPGGameData::ParseGameDataByXMLDoc(xmlutils::MyXMLDoc* document)
{

    xmlutils::MyXMLNode dialog_node=document->GetNode("/ermaze/game/dialog");
    ge_common_struct::dialog_style_node style=ge_fileutil
            ::parse_dialog_style(dialog_node);
    m_dialog_style=style;
    xmlutils::MyXMLNode start_node=document->GetNode("/ermaze/game/startpoint");
    ge_common_struct::start_point2d start_point;
    m_current_scene=start_node.StrAttribute("scene");
    start_point.start_scene=m_current_scene;
    start_point.x=start_node.IntAttribute("x");
    start_point.y=start_node.IntAttribute("y");
    start_point.direction=start_node.StrAttribute("direction");
    start_point.layer=start_node.IntAttribute("layer");
    SetStartPoint(start_point);

    xmlutils::MyXMLNode sheet_node=document->
                                   GetNode("/ermaze/game/spritesheets/spritesheet");

    for(; sheet_node; sheet_node=sheet_node.NextSlibing("spritesheet"))
    {
        std::string node_id =sheet_node.StrAttribute("id");
        std::string path="./sprites/"+sheet_node.StrAttribute("path");
        int width=sheet_node.IntAttribute("width");
        int height=sheet_node.IntAttribute("height");
        int col=sheet_node.IntAttribute("col");
        int row=sheet_node.IntAttribute("row");
        CSpriteSheet* sheet=new CSpriteSheet(path,width,height,col,row);
        m_sprite_sheets[node_id]=sheet;
    }

    xmlutils::MyXMLNode character_node=document->
                                       GetNode("/ermaze/game/characters/character");

    for(; character_node; character_node=character_node.NextSlibing("character"))
    {
        std::string id=character_node.StrAttribute("id");
        std::string sheet_id=character_node.StrAttribute("sheet_id");
        AddSprite(id,sheet_id);
        xmlutils::MyXMLNode actionsnode=character_node.Child("actions");
        if(actionsnode)
        {
            xmlutils::MyXMLNode action_node=actionsnode.Child("action");
            for(; action_node; action_node=action_node.NextSlibing("action"))
            {
                std::string action_name=action_node.StrAttribute("name");
                std::string actiontile=action_node.StrAttribute("ids");
                std::vector<int> ids=ge_str_utilities::
                                     SplitStrToIntArray(actiontile,',');
                AddAction(id,action_name,ids);

            }
        }
    }

    xmlutils::MyXMLNode player_node=document->
                                    GetNode("/ermaze/game/players/player");

    //Single Player
    xmlutils::MyXMLNode char_node=player_node.Child("character");

    for(; char_node; char_node=char_node.NextSlibing("character"))
    {
        std::string character=char_node.StrAttribute("refid");
        AddPlayer(character);
    }


    xmlutils::MyXMLNode scene_node= document->GetNode("/ermaze/game/scenes/scene");
    for(; scene_node; scene_node=scene_node.NextSlibing("scene"))
    {
        std::string node_id=scene_node.StrAttribute("id");
        xmlutils::MyXMLNode tilemap=scene_node.Child("tilemap");
        CSceneData scene;
        if(tilemap)
        {
            std::string tilepath=tilemap.StrAttribute("name");
            std::string collide_prop=tilemap.StrAttribute("collide_prop");
            std::string layerswitch_prop=tilemap.StrAttribute("layerswitch_prop");

            scene.SetTileMapPath(tilepath);
            scene.SetCollideProp(collide_prop);
            scene.SetLayerSwitchProp(layerswitch_prop);
        }
        m_scene_list[node_id]=scene;
    }

}
