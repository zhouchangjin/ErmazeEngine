#include "CGameScene.h"

CGameScene::CGameScene()
{
    //ctor
}


CGameScene::CGameScene(const CGameScene& gamescene){
    m_tilemap_path=gamescene.GetTileMapPath();
    m_collide_prop=gamescene.GetTileCollideProp();
    m_layerswitch_prop=gamescene.GetLayerSwitchProp();
}

const std::string CGameScene::GetTileMapPath()const{
    const std::string path=m_tilemap_path;
    return path;
}

bool CGameScene::GetTileCollideBool(int tile_id){
   if(m_collidable_tiles.find(tile_id)!=m_collidable_tiles.end()){
        return true;
   }else{
        return false;
   }
}

bool CGameScene::GetSwitchTileBool(int tile_id){
    if(m_layerswitches.find(tile_id)!=m_layerswitches.end()){
        return true;
    }else{
        return false;
    }
}

void CGameScene::AddCollideTile(int tile_id){
    m_collidable_tiles[tile_id]=true;
}

void CGameScene::AddSwitchTile(int tile_id){
    m_layerswitches[tile_id]=true;
}

CGameScene::~CGameScene()
{
    //dtor
}
