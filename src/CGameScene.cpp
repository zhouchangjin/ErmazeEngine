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

CGameScene::~CGameScene()
{
    //dtor
}
