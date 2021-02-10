#include "CImageDB.h"

CImageDB::CImageDB()
{
    //ctor
}

CImageDB::~CImageDB()
{
    for(size_t i=0; i<m_spritesheets.size(); i++)
    {
        CSpriteSheet* sheet=m_spritesheets[i];
        delete sheet;
    }
    m_spritesheets.clear();
}

void CImageDB::AddTexture(std::string sheet_name,
                            std::string texture_name,int idx){
    CTiledTexture& tiled_texture=m_textures[sheet_name];
    tiled_texture.AddTexture(texture_name,idx);
    m_texture_map[texture_name]=sheet_name;
}

void CImageDB::AddSpriteSheet(CSpriteSheet* spritesheet,
                              std::string sheet_name){
    //不支持多线程
    m_sheet_map[sheet_name]=m_spritesheets.size();
    m_spritesheets.push_back(spritesheet);
    CTiledTexture tile_icon(spritesheet);
    m_textures[sheet_name]=tile_icon;
}

CTexture CImageDB::GetTexture(std::string texture_name){
    if(m_texture_map.find(texture_name)!=m_texture_map.end()){
        std::string sheet_name=m_texture_map[texture_name];
        CTiledTexture* tt=&m_textures[sheet_name];
        int idx=tt->GetTexturePos(texture_name);
        CTexture texture(tt,idx);
        return texture;
    }else{
        return CTexture(nullptr,-1);
    }
    //CIcon icon()
}

bool CImageDB::ContainsSheet(std::string sheet_name){
    if(m_textures.find(sheet_name)!=m_textures.end()){
        return true;
    }else{
        return false;
    }
}

bool CImageDB::ContainsTexture(std::string texture_name){
    if(m_texture_map.find(texture_name)!=m_texture_map.end()){
        return true;
    }else{
        return false;
    }
}

CTiledTexture CImageDB::GetTiledTexture(std::string texture_name){
    if(m_texture_map.find(texture_name)!=m_texture_map.end()){
        std::string iconsheet=m_texture_map[texture_name];
        return m_textures[iconsheet];
    }else{
        return CTiledTexture(nullptr);
    }
}
