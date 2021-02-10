#include "CTiledTexture.h"

CTiledTexture::CTiledTexture()
{
    //ctor
}

CTiledTexture::~CTiledTexture()
{
    //dtor
}

void CTiledTexture::AddTexture(std::string texture_name,int texture_idx){
    std::vector<int> texture_idxs;
    texture_idxs.push_back(texture_idx);
    AddAction(texture_name,texture_idxs);

}

int CTiledTexture::GetTexturePos(std::string texture_name){
    if(m_action_map.find(texture_name)!=m_action_map.end()){
        return m_action_map[texture_name][0];
    }else{
        return 0;
    }
}
