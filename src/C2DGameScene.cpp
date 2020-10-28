#include "C2DGameScene.h"

C2DGameScene::C2DGameScene()
{
    //ctor
}

C2DGameScene::~C2DGameScene()
{

}

C2DGameScene::C2DGameScene(const C2DGameScene& copyscene){

}


bool C2DGameScene::GetTileCollideBool(int tile_id){
   if(m_collidable_tiles.find(tile_id)!=m_collidable_tiles.end()){
        return true;
   }else{
        return false;
   }
}

bool C2DGameScene::GetSwitchTileBool(int tile_id){
    if(m_layerswitches.find(tile_id)!=m_layerswitches.end()){
        return true;
    }else{
        return false;
    }
}

void C2DGameScene::AddCollideTile(int tile_id){
    m_collidable_tiles[tile_id]=true;
}

void C2DGameScene::AddSwitchTile(int tile_id){
    m_layerswitches[tile_id]=true;
}

CTileLayer* C2DGameScene::GetTileLayer(int layer_no){

    CLayer* layer=GetLayer(layer_no);
    if(layer->GetLayerType()==ge_common_struct::layer_type::TILED_LAYER){

        return (CTileLayer*) layer;

    }else{
        return NULL;
    }
}


