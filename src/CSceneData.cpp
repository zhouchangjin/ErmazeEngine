#include "CSceneData.h"

CSceneData::CSceneData()
{
    //ctor
}

CSceneData::~CSceneData()
{
    //dtor
}

const std::string CSceneData::GetTileMapPath()const{
    const std::string path=m_tilemap_path;
    return path;
}

CSceneData::CSceneData(const CSceneData& copyscene){
    m_tilemap_path=copyscene.GetTileMapPath();
    m_collide_prop=copyscene.GetTileCollideProp();
    m_layerswitch_prop=copyscene.GetLayerSwitchProp();
}

