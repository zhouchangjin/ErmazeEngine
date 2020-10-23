#include "CTileLayer.h"
CTileLayer::CTileLayer()
{
    m_type=ge_common_struct::layer_type::TILED_LAYER;
}

CTileLayer::~CTileLayer()
{
    //dtor
}

void CTileLayer::AddRow(ge_common_struct::ROW_IDX row){
    m_tiles.push_back(row);
}

ge_common_struct::LAYER_IDX CTileLayer::GetTiles(){
    return m_tiles;
}
