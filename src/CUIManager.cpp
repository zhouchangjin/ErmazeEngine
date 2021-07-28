#include "CUIManager.h"

CUIManager::CUIManager()
{
    //ctor
}

CUIManager::~CUIManager()
{
    //dtor
}


void CUIManager::AddPanel(std::string panel_name,
                          ge_common_struct::dom_node* panel,bool is_hide,bool is_pop)
{
    m_panels[panel_name]=panel;
    panel->style.visibility=!is_hide;
    if(is_pop)
    {
        m_pop_panel_list.push_back(panel_name);
    }
    else
    {
        m_panel_list.push_back(panel_name);
    }
}

void CUIManager::Register(std::string name,IMenuProcess* instance)
{
    m_action_manager.Register(name,instance);
}

void CUIManager::Init()
{

    m_database=CServiceLocator::
               GetService<CGameDatabase>
               (CServiceLocator::ServiceID::DATABASE);
    m_imagedb=CServiceLocator::GetService<CImageDB>
              (CServiceLocator::ServiceID::TEXTURE_DB);

}

void CUIManager::Draw()
{

    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    sdlutil2::FillRect(m_context,fullWindow,0,0,0,255);
    //绘制界面
    for(size_t i=0; i<m_panel_list.size(); i++)
    {
        std::string panel_name=m_panel_list[i];
        ge_common_struct::dom_node* p=m_panels[panel_name];
        int page_start=0;
        if(m_panel_page_start.find(panel_name)!=m_panel_page_start.end())
        {
            page_start=m_panel_page_start[panel_name];
        }
        sdlutil2::UpdateDomNode(p,m_database,-1,page_start);
        sdlutil2::UpdateDomRect(p,fullWindow);
        sdlutil2::DrawDomNode(m_context,p,m_imagedb);
    }
    //绘制弹出窗口
    for(size_t i=0; i<m_pop_panel_stack.size(); i++)
    {
        std::string panel_name=m_pop_panel_stack[i];
        ge_common_struct::dom_node* p=m_panels[panel_name];
        int page_start=0;
        if(m_panel_page_start.find(panel_name)!=m_panel_page_start.end())
        {
            page_start=m_panel_page_start[panel_name];
        }
        if(p->style.visibility)
        {
            sdlutil2::UpdateDomNode(p,m_database,-1,page_start);
            sdlutil2::UpdateDomRect(p,fullWindow);
            sdlutil2::DrawDomNode(m_context,p,m_imagedb);
            if(i==m_pop_panel_stack.size()-1)
            {
                CTiledTexture icon=m_imagedb->GetTiledTexture(m_menu_pointer_name);
                sdlutil2::DrawTexture(m_context,p,m_el_pointer,icon,m_menu_pointer_name
                                      ,10,0,2);
            }

        }

    }
}

void CUIManager::SetPanelList(std::string panel_name,std::string list_name)
{
    m_panels[panel_name]->list_name=list_name;
}

void CUIManager::SetPanelObject(std::string panel_name,int obj_id)
{
    m_panels[panel_name]->obj_id=obj_id;
}

void CUIManager::ShowPopPanel(std::string panel_name)
{
    if(m_panels.find(panel_name)!=m_panels.end())
    {
        //保存并隐藏上一个窗口状态
        if(m_pop_panel_stack.size()>0)
        {
            std::string last_panel=m_pop_panel_stack.back();
            ge_common_struct::dom_node* last_panel_node=m_panels[last_panel];
            if(last_panel_node->type.compare("menu")==0)
            {

            }
            else
            {
                last_panel_node->style.visibility=false;
            }
            m_elp_stack.push_back(m_el_pointer);
        }
        //显示新窗口
        m_pop_panel_stack.push_back(panel_name);
        ge_common_struct::dom_node* panel_node=m_panels[panel_name];
        panel_node->style.visibility=true;
        if(ge_common_struct::DomEnablePage(panel_node))
        {
            m_panel_page_start[panel_name]=0;
        }
        m_el_pointer=0;
    }
}

bool CUIManager::IsPopPanelHidden()
{
    bool res=true;
    for(size_t i=0; i<m_pop_panel_list.size(); i++)
    {
        std::string panel_name=m_pop_panel_list[i];
        bool vis=m_panels[panel_name]->style.visibility;
        if(vis)
        {
            res=false;
            break;
        }
    }
    return res;
}


void CUIManager::HideAllPopPanel()
{
    m_el_pointer=0;
    if(m_pop_panel_stack.size()>0)
    {
        for(size_t i=0; i<m_pop_panel_stack.size(); i++)
        {
            std::string panel_name=m_pop_panel_stack[i];
            ge_common_struct::dom_node* node=m_panels[panel_name];
            node->style.visibility=false;
        }
        m_pop_panel_stack.clear();
    }
    if(m_elp_stack.size()>0)
    {

        m_elp_stack.clear();
    }
    if(m_current_command.size()>0){
        m_last_command=m_current_command;
        m_current_command.clear();
    }
}

void CUIManager::CloseCurrentPopPanel()
{
    if(m_pop_panel_stack.size()==0)
    {
        return;
    }
    //Hide current Panel
    std::string panel_name=m_pop_panel_stack.back();
    m_pop_panel_stack.pop_back();
    ge_common_struct::dom_node* panel_node=m_panels[panel_name];
    panel_node->style.visibility=false;

    if(m_elp_stack.size()>0)
    {
        //Show Last Panel with Correct Selection
        if(m_elp_stack.size()==m_current_command.size()){
            //ensure m_current_command size is equal to m_elp_stack size
            m_current_command.pop_back();
        }else{
            //TODO　some exception happens
        }
        m_el_pointer=m_elp_stack.back();
        m_elp_stack.pop_back();
        std::string last_panel_name=m_pop_panel_stack.back();
        ge_common_struct::dom_node* last_panel_node=m_panels[last_panel_name];
        last_panel_node->style.visibility=true;
    }
    else
    {
        m_el_pointer=0;
        m_current_command.clear();
        m_last_command.clear();
    }
}

void CUIManager::ProcessInput(CMenuInputEvent event)
{

    ge_common_struct::key_event kevent=event.GetInput().get_top_event();
    ge_common_struct::key_event_type event_type=kevent.key;
    ge_common_struct::key_press_type press_type=kevent.press;
    if(press_type==ge_common_struct::key_press_type::KEY_PRESS
           /* || press_type==ge_common_struct::key_press_type::KEY_PRESSED*/)
    {

        int state_value=event.GetCurrentSubState();

        if(state_value==CUIManager::UIState::POP_INIT)
        {
            GE_LOG("UIManager Show Panel Event\n");
            ShowPopPanel(event.GetMenuInitPanel());
        }
        else if(state_value==CUIManager::UIState::POP_MENU)
        {

            if(m_pop_panel_stack.size()>0)
            {
                std::string panel_name=m_pop_panel_stack.back();
                if(m_panels.find(panel_name)!=m_panels.end())
                {
                    ge_common_struct::dom_node* node=m_panels[panel_name];
                    bool enable_page=ge_common_struct::DomEnablePage(node);
                    int cnt=ge_common_struct::CntDomChild(node);
                    int max_cnt=cnt;
                    if(enable_page)
                    {
                        max_cnt=sdlutil2::GetMaxPageCntOfDomNode(node,m_database);
                    }
                    if(cnt>0)
                    {
                        int col=node->col;
                        ge_common_struct::ui_layout layout=node->child_layout;

                        if(event_type==ge_common_struct::key_event_type::KEY_DOWN)
                        {
                            if(layout==ge_common_struct::ui_layout::GRID_LAYOUT)
                            {
                                m_el_pointer+=col;
                            }
                            else
                            {
                                m_el_pointer++;
                            }
                            if(enable_page)
                            {
                                if(m_el_pointer>=cnt)
                                {
                                    m_el_pointer=m_el_pointer-1;
                                    int& page_start=m_panel_page_start[panel_name];
                                    if(page_start+cnt<max_cnt)
                                    {
                                        page_start++;
                                    }
                                }
                            }
                            else
                            {
                                m_el_pointer=m_el_pointer%cnt;
                            }

                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_UP)
                        {
                            if(layout==ge_common_struct::ui_layout::GRID_LAYOUT)
                            {
                                m_el_pointer-=col;
                            }
                            else
                            {
                                m_el_pointer--;
                            }
                            if(enable_page)
                            {
                                if(m_el_pointer<0)
                                {
                                    m_el_pointer=0;
                                    int& page_start=m_panel_page_start[panel_name];
                                    if(page_start>0)
                                    {
                                        page_start--;
                                    }
                                }
                            }
                            else
                            {
                                m_el_pointer=(m_el_pointer+cnt)%cnt;
                            }
                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_LEFT)
                        {
                            m_el_pointer--;
                            if(enable_page)
                            {
                                if(m_el_pointer<0)
                                {
                                    m_el_pointer=0;
                                    int& page_start=m_panel_page_start[panel_name];
                                    if(page_start>0)
                                    {
                                        page_start--;
                                    }
                                }


                            }
                            else
                            {
                                m_el_pointer=(m_el_pointer+cnt)%cnt;
                            }

                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_RIGHT)
                        {
                            m_el_pointer++;
                            if(enable_page)
                            {
                                if(m_el_pointer>=cnt)
                                {
                                    m_el_pointer=m_el_pointer-1;
                                    int& page_start=m_panel_page_start[panel_name];
                                    if(page_start+cnt<max_cnt)
                                    {
                                        page_start++;
                                    }
                                }

                            }
                            else
                            {
                                m_el_pointer=m_el_pointer%cnt;
                            }

                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_CONFIRM)
                        {
                            ge_common_struct::dom_node* sel
                                =ge_common_struct::GetDomSelection(node,m_el_pointer);
                            IMenuProcess * process=m_action_manager.GetMenuInterface(panel_name);
                            if(sel)
                            {
                                int obj_id=sel->obj_id;
                                ge_common_struct::menu_command command;
                                command.menu_name=panel_name;
                                command.menu_order=m_el_pointer;
                                command.obj_id=obj_id;
                                command.command_name=sel->action_type_name;
                                m_current_command.push_back(command);
                                if(process)
                                {
                                    process->Choose(obj_id,m_el_pointer);
                                }
                                if(sel->action_name.compare("")==0)
                                {
                                    //如果没有子菜单隐藏所有菜单。
                                    //command generated
                                    HideAllPopPanel();
                                }
                                else
                                {
                                    ShowPopPanel(sel->action_name);
                                }
                            }


                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_CANCLE)
                        {
                            CloseCurrentPopPanel();
                        }
                    }
                    else
                    {
                        CloseCurrentPopPanel();
                    }
                }
            }


        }
    }

}
