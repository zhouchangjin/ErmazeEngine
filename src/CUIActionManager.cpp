#include "CUIActionManager.h"

CUIActionManager::CUIActionManager()
{
    //ctor
}

CUIActionManager::~CUIActionManager()
{
    //dtor
}

void CUIActionManager::Register(std::string name,IMenuProcess* instance){
    m_ui_action[name]=instance;
}

IMenuProcess* CUIActionManager::GetMenuInterface(std::string name){
    if(m_ui_action.find(name)!=m_ui_action.end()){
        return m_ui_action[name];
    }else{
        return nullptr;
    }
}
