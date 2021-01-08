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

void CImageDB::AddIconSheet(std::string sheet_name,
                            std::string icon_name,int icon_idx){
    CTiledIcon& tiled_icon=m_icons[sheet_name];
    tiled_icon.AddIcon(icon_name,icon_idx);
    m_icon_map[icon_name]=sheet_name;
}

void CImageDB::AddSpriteSheet(CSpriteSheet* spritesheet,
                              std::string sheet_name){
    //不支持多线程
    m_sheet_map[sheet_name]=m_spritesheets.size();
    m_spritesheets.push_back(spritesheet);
    CTiledIcon tile_icon(spritesheet);
    m_icons[sheet_name]=tile_icon;
}

CIcon CImageDB::GetIcon(std::string icon_name){
    if(m_icon_map.find(icon_name)!=m_icon_map.end()){
        std::string icon_sheet=m_icon_map[icon_name];
        CTiledIcon* ticon=&m_icons[icon_sheet];
        int idx=ticon->GetIconPos(icon_name);
        CIcon icon(ticon,idx);
        return icon;
    }else{
        return CIcon(nullptr,-1);
    }
    //CIcon icon()
}

bool CImageDB::ContainsSheet(std::string sheet_name){
    if(m_icons.find(sheet_name)!=m_icons.end()){
        return true;
    }else{
        return false;
    }
}

bool CImageDB::ContainsIcon(std::string icon_name){
    if(m_icon_map.find(icon_name)!=m_icon_map.end()){
        return true;
    }else{
        return false;
    }
}

CTiledIcon CImageDB::GetTiledIcon(std::string icon_name){
    if(m_icon_map.find(icon_name)!=m_icon_map.end()){
        std::string iconsheet=m_icon_map[icon_name];
        return m_icons[iconsheet];
    }else{
        return CTiledIcon(nullptr);
    }
}
