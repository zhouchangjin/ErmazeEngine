#include "CGameScene.h"

CGameScene::CGameScene()
{
    //ctor
}


CGameScene::CGameScene(const CGameScene& gamescene){
    m_tilemap_path=gamescene.GetTileMapPath();
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

void CGameScene::AddCollideTile(int tile_id){
    m_collidable_tiles[tile_id]=true;
}

CGameScene::~CGameScene()
{
    //dtor
}
