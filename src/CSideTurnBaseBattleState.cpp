#include "CSideTurnBaseBattleState.h"

CSideTurnBaseBattleState::CSideTurnBaseBattleState()
{
    //ctor
}

CSideTurnBaseBattleState::CSideTurnBaseBattleState(CGameContext* context)
:CGameState(context)
{

}

CSideTurnBaseBattleState::~CSideTurnBaseBattleState()
{
    //dtor release m_panels m_menu
}

void CSideTurnBaseBattleState::Init()
{
    //0代表状态不改变
    m_state_value=0;
    LoadComponents();
    LoadUIDef();

}

void CSideTurnBaseBattleState::Cleanup()
{

}

void CSideTurnBaseBattleState::Pause()
{

}

void CSideTurnBaseBattleState::Resume()
{
    m_state_value=0;
}

void CSideTurnBaseBattleState::HandleEvent(ge_common_struct::input_event event)
{
}

void CSideTurnBaseBattleState::Update()
{

}

void CSideTurnBaseBattleState::Draw()
{
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    sdlutil2::FillRect(m_context,fullWindow,0,0,0,255);
    std::map<std::string,ge_common_struct::dom_node*>::iterator it;
    for(it=m_panels.begin(); it!=m_panels.end(); it++)
    {
        ge_common_struct::dom_node* p=it->second;
        sdlutil2::UpdateDomRect(p,fullWindow);
        sdlutil2::DrawDomNode(m_context,p,m_imagedb);


    }
    sdlutil2::RenderPresent(m_context);

}

int CSideTurnBaseBattleState::GetStateValue()
{
    return this->m_state_value;
}

void CSideTurnBaseBattleState::PrepareData()
{

}

void CSideTurnBaseBattleState::LoadComponents()
{
    m_database=CServiceLocator::
               GetService<CGameDatabase>
               (CServiceLocator::ServiceID::DATABASE);
    m_imagedb=CServiceLocator::GetService<CImageDB>
              (CServiceLocator::ServiceID::TEXTURE_DB);

}

void CSideTurnBaseBattleState::LoadUIDef()
{

    xmlutils::MyXMLDoc doc=xmlutils::LoadXML("./ui/battle.ui");
    xmlutils::MyXMLNode node=doc.GetNode("ui");
    xmlutils::MyXMLNode hud=node.Child("hud");
    xmlutils::MyXMLNode menu=node.Child("menu");

    xmlutils::MyXMLNode panel=hud.Child("panel");
    xmlutils::MyXMLNode window=menu.Child("window");

    int id=0;
    std::string idStr="";
    for(; panel; panel=panel.NextSlibing("panel"))
    {
        id++;
        ge_common_struct::dom_node* dom_node_ptr=ge_fileutil::parse_dom(panel);
        if(panel.HasAttribute("id"))
        {
            idStr=panel.StrAttribute("id");
        }
        else
        {
            idStr+=std::to_string(id);
        }
        m_panels[idStr]=dom_node_ptr;
    }
    for(; window; window=window.NextSlibing("window"))
    {
        id++;
        ge_common_struct::dom_node* dom_node_ptr=ge_fileutil::parse_dom(window);
        if(panel.HasAttribute("id"))
        {
            idStr=panel.StrAttribute("id");
        }
        else
        {
            idStr+=std::to_string(id);
        }
        m_menus[idStr]=dom_node_ptr;
    }

}
