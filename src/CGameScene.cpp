#include "CGameScene.h"

CGameScene::CGameScene()
{
    //ctor
}


CGameScene::CGameScene(const CGameScene& gamescene){

}

CGameScene::~CGameScene()
{
    //dtor
}

void CGameScene::ClearScene(){
    for(size_t i=0;i<m_layer_list.size();i++){
        delete m_layer_list[i];
    }
    m_layer_list.clear();
}

int CGameScene::GetLayerCnt(){
    return m_layer_list.size();
}

void CGameScene::AddLayer(CLayer* layer){
    m_layer_list.push_back(layer);
}

CLayer* CGameScene::GetLayer(int layer_no){
   return m_layer_list[layer_no];
}
