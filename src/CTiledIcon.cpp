#include "CTiledIcon.h"

CTiledIcon::CTiledIcon()
{
    //ctor
}

CTiledIcon::~CTiledIcon()
{
    //dtor
}

void CTiledIcon::AddIcon(std::string icon_name,int icon_idx){
    std::vector<int> icon_idxs;
    icon_idxs.push_back(icon_idx);
    AddAction(icon_name,icon_idxs);

}

int CTiledIcon::GetIconPos(std::string icon_name){
    if(m_action_map.find(icon_name)!=m_action_map.end()){
        return m_action_map[icon_name][0];
    }else{
        return 0;
    }
}
