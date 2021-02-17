#include "CImageDB.h"

CImageDB::CImageDB()
{
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
                          std::string texture_name,int idx)
{
    CTiledTexture& tiled_texture=m_textures[sheet_name];
    tiled_texture.AddTexture(texture_name,idx);
    m_texture_map[texture_name]=sheet_name;
}

void CImageDB::AddSpriteSheet(CSpriteSheet* spritesheet,
                              std::string sheet_name)
{
    //不支持多线程
    m_sheet_map[sheet_name]=m_spritesheets.size();
    m_spritesheets.push_back(spritesheet);
    CTiledTexture tile_icon(spritesheet);
    m_textures[sheet_name]=tile_icon;
}

CTexture CImageDB::GetTexture(std::string texture_name)
{
    if(m_texture_map.find(texture_name)!=m_texture_map.end())
    {
        std::string sheet_name=m_texture_map[texture_name];
        CTiledTexture* tt=&m_textures[sheet_name];
        int idx=tt->GetTexturePos(texture_name);
        CTexture texture(tt,idx);
        return texture;
    }
    else
    {
        return CTexture(nullptr,-1);
    }
    //CIcon icon()
}

bool CImageDB::ContainsSheet(std::string sheet_name)
{
    if(m_textures.find(sheet_name)!=m_textures.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CImageDB::ContainsTexture(std::string texture_name)
{
    if(m_texture_map.find(texture_name)!=m_texture_map.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

CTiledTexture CImageDB::GetTiledTexture(std::string texture_name)
{
    if(m_texture_map.find(texture_name)!=m_texture_map.end())
    {
        std::string iconsheet=m_texture_map[texture_name];
        return m_textures[iconsheet];
    }
    else
    {
        return CTiledTexture(nullptr);
    }
}

void CImageDB::Initialize()
{

    xmlutils::MyXMLDoc doc=xmlutils::LoadXML("./ui/resource.ui");
    xmlutils::MyXMLNode sheet_node=doc.
                                   GetNode("/ui/spritesheets");
    xmlutils::MyXMLNode icon_node=doc.GetNode("/ui/icons");

    std::map<std::string,ge_common_struct::image_def> sheets;
    std::map<std::string,ge_common_struct::icon_def> icons;
    ge_fileutil::parse_sheets(sheet_node,sheets);
    ge_fileutil::parse_icons(icon_node,icons);
    std::map<std::string,ge_common_struct::image_def>::iterator it;
    std::map<std::string,ge_common_struct::icon_def>::iterator it_icon;

    for(it=sheets.begin(); it!=sheets.end(); it++)
    {
        ge_common_struct::image_def image=it->second;
        CSpriteSheet* sprite_sheet=new CSpriteSheet(image.path,image.width,
                image.height,
                image.col,image.row);
        AddSpriteSheet(sprite_sheet,image.id);
    }

    for(it_icon=icons.begin(); it_icon!=icons.end(); it_icon++)
    {
        ge_common_struct::icon_def icon=it_icon->second;
        std::string sheet_id=icon.resource_id;
        std::string icon_name=icon.icon_name;
        int icon_idx=icon.id;
        if(ContainsSheet(sheet_id))
        {
            AddTexture(sheet_id,icon_name,icon_idx);
        }
    }

}
