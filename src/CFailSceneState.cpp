#include "CFailSceneState.h"

CFailSceneState::CFailSceneState()
{
    //ctor
}

CFailSceneState::~CFailSceneState()
{
    //dtor
}

CFailSceneState::CFailSceneState(CGameContext* context)
{
    m_context=context;
}

void CFailSceneState::Init()
{
    m_ui_manager.SetGameContext(m_context);
    m_ui_manager.Init();

    xmlutils::MyXMLDoc doc=xmlutils::LoadXML("./ui/die.ui");
    xmlutils::MyXMLNode node=doc.GetNode("ui");
    xmlutils::MyXMLNode hud=node.Child("hud");
    xmlutils::MyXMLNode panel=hud.Child("panel");

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
        m_ui_manager.AddPanel(idStr,dom_node_ptr);
    }
    idStr="";

    xmlutils::MyXMLNode menu=node.Child("menu");
    if(menu)
    {
        xmlutils::MyXMLNode window=menu.Child("window");
        for(; window; window=window.NextSlibing("window"))
        {
            id++;
            ge_common_struct::dom_node* dom_node_ptr=ge_fileutil::parse_dom(window);
            if(window.HasAttribute("id"))
            {
                idStr=window.StrAttribute("id");
            }
            else
            {
                idStr+=std::to_string(id);
            }
            m_ui_manager.AddPanel(idStr,dom_node_ptr,true,true);
        }

    }


}

void CFailSceneState::Update()
{

}

void CFailSceneState::Cleanup()
{

}

void CFailSceneState::Pause()
{

}

void CFailSceneState::Resume()
{
m_state_value=0;
}

void CFailSceneState::HandleEvent(ge_common_struct::input_event event)
{
}

void CFailSceneState::Draw()
{

    m_ui_manager.Draw();
    sdlutil2::RenderPresent(m_context);
}

int CFailSceneState::GetStateValue()
{
    return this->m_state_value;
}

void CFailSceneState::PrepareData()
{


}
