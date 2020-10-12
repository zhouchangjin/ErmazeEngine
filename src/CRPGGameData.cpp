#include "CRPGGameData.h"
//#include <iostream>
CRPGGameData::CRPGGameData()
{
    //ctor
}

CRPGGameData::~CRPGGameData()
{
    //dtor
}

CGameScene CRPGGameData::GetCurrentScene(){
    return m_scene_list[m_current_scene];
}

void CRPGGameData::ParseGameDataByXMLDoc(xmlutils::MyXMLDoc* document){
   m_current_scene="01"; //hard coding;
   xmlutils::MyXMLNode node= document->GetNode("/ermaze/game/scenes/scene");
   for(;node;node=node.NextSlibing("scene")){
       std::string node_id=node.StrAttribute("id");
       xmlutils::MyXMLNode tilemap=node.Child("tilemap");
       std::string tilepath=tilemap.StrAttribute("name");
       std::string collide_prop=tilemap.StrAttribute("collide_prop");
       //std::cout<<node_id<<"--"<<tilepath<<std::endl;
       CGameScene scene;
       scene.SetTileMapPath(tilepath);
       scene.SetCollideProp(collide_prop);
       //m_scene_list.insert(std::pair<std::string,CGameScene>("",scene));
       m_scene_list[node_id]=scene;
   }

}
