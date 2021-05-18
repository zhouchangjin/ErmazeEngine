#include "CGameUISystem.h"
CGameUISystem::CGameUISystem()
{
}

CGameUISystem::CGameUISystem(CGameContext* context)
{
    m_context=context;
}

CGameUISystem::~CGameUISystem()
{
    std::map<std::string,ge_common_struct::dom_node*>::iterator it;
    for(it=m_panels.begin(); it!=m_panels.end(); it++)
    {
        ge_common_struct::dom_node* domnode=it->second;
        ge_common_struct::FreeDomNode(domnode);
    }
    m_panels.clear();
}

void CGameUISystem::SetDialogStyle(ge_common_struct::dialog_style_node style)
{
    m_dialog_style=style;
    ge_common_struct::ge_adv_color c=m_dialog_style.main_window.background_color;
    ge_common_struct::ge_color fc=m_dialog_style.main_window.font_color;
    ge_common_struct::ge_adv_color bc=m_dialog_style.main_window.border_color;
    int bw=style.main_window.border_width;
    //m_dialog.SetBackGroundColor(0,0,0,200);
    //m_dialog.SetFontColor(255,255,255);
    m_dialog.SetBackGroundColor(c.r,c.g,c.b,c.a);
    m_dialog.SetFontColor(fc.r,fc.g,fc.b);
    m_dialog.SetBorderColor(bc.r,bc.g,bc.b);
    m_dialog.SetBorderWidth(bw);
    m_dialog.SetDisplaySpeed(3);
    m_dialog.ShowIndicator();//永远显示提示指针比较方便

}

void CGameUISystem::LoadUI()
{

    xmlutils::MyXMLDoc doc=xmlutils::LoadXML("./ui/menu.ui");
    xmlutils::MyXMLNode node=doc.GetNode("ui");

    xmlutils::MyXMLNode window=node.Child("window");
    int id=0;
    std::string idStr="";
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
        m_panels[idStr]=dom_node_ptr;

    }

}

void CGameUISystem::LoadDatabase()
{

    m_database=CServiceLocator::
               GetService<CGameDatabase>
               (CServiceLocator::ServiceID::DATABASE);
    m_imagedb=CServiceLocator::GetService<CImageDB>
    (CServiceLocator::ServiceID::TEXTURE_DB);
}

bool CGameUISystem::EventLock()
{
    if(m_dialog.IsShow())
    {
        return true;
    }
    else
    {
        if(m_menu_stack.size()>0)
        {
            return true;
        }
        else
        {
            return false;
        }

    }
}


void CGameUISystem::UpdateDialogStyle()
{
    ge_common_struct::ge_rect r=m_dialog_style.main_window.client_rect;
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    int dialog_x=0,dialog_y=0,dialog_w=0,dialog_h=0;
    if(m_dialog_style.main_window.is_percentage)
    {
        dialog_x=fullWindow.x+(fullWindow.w*r.x/100);
        dialog_y=fullWindow.y+(fullWindow.h*r.y/100);
        dialog_w=fullWindow.w*r.w/100;
        dialog_h=fullWindow.h*r.h/100;
    }
    else
    {
        dialog_x=fullWindow.x+r.x;
        dialog_y=fullWindow.y+r.y;
        dialog_w=r.w;
        dialog_h=r.h;
    }
    m_dialog.SetX(dialog_x);
    m_dialog.SetY(dialog_y);
    m_dialog.SetWidth(dialog_w);
    m_dialog.SetHeight(dialog_h);
    //m_dialog.SetTitleHeight(50);
    //m_dialog.SetTitleWidth(200);
    //m_dialog.SetTitleX(fullWindow.x);
    //m_dialog.SetTitleY(m_dialog.GetY()-m_dialog.GetTitleHeight()
    //                   +m_dialog.GetBorderWidth());
    m_dialog.InitResponseByDialog();
}

CTiledTexture CGameUISystem::GetTileIcon(std::string icon_name)
{
    return m_imagedb->GetTiledTexture(icon_name);
}

void CGameUISystem::Draw()
{
    UpdateDialogStyle();
    if(m_dialog.IsShow())
    {
        sdlutil2::DrawAdvDialog(m_context,m_dialog);
    }
    else
    {
        if(m_menu_stack.size()>0)
        {
            ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
            std::string menu_id=m_menu_stack.back();
            if(m_panels.find(menu_id)!=m_panels.end())
            {
                ge_common_struct::dom_node* menu=m_panels[menu_id];
                sdlutil2::UpdateDomRect(menu,fullWindow);
                sdlutil2::DrawDomNode(m_context,menu,m_imagedb);
                CTiledTexture icon=GetTileIcon(m_menu_pointer);
                sdlutil2::DrawTexture(m_context,menu,m_el_pointer,icon,m_menu_pointer,-10,0,m_icon_scale);
            }
        }

    }
}

void CGameUISystem::SelChoice()
{
    size_t sel_id=m_dialog.GetChoiceDialog()->GetSelection();
    m_cursor_node=m_cursor_node->children[sel_id];
    m_line_no=0;
    LoadNextLine();
}

bool CGameUISystem::ConditionCheck(ge_common_struct::exp_node* exp)
{
    if(exp->left==nullptr)
    {
        //is roof
        int db_value=-1;
        std::string attribute_name=exp->cond.attribute_name;
        size_t current=attribute_name.find(".");
        if(current!=std::string::npos)
        {
            std::string obj_name=attribute_name.substr(0,current);
            std::string prop_name=attribute_name.substr(current+1);
            int obj_id=m_database->GetObjectId(obj_name);
            db_value=m_database->GetObjectData(obj_id,prop_name);
        }
        else
        {
            db_value=m_database->GetIntData(exp->cond.attribute_name);
        }
        ge_common_struct::condition_type type=exp->cond.type;
        if(type==ge_common_struct::condition_type::EQUAL)
        {
            return db_value==exp->cond.attribute_value;
        }
        else if(type==ge_common_struct::condition_type::GREATER)
        {
            return db_value>exp->cond.attribute_value;
        }
        else if(type==ge_common_struct::condition_type::LESS)
        {
            return db_value<exp->cond.attribute_value;
        }
        else if(type==ge_common_struct::condition_type::EQUAL_GREATER)
        {
            return db_value>=exp->cond.attribute_value;
        }
        else if(type==ge_common_struct::condition_type::EQUAL_LESS)
        {
            return db_value<=exp->cond.attribute_value;
        }
        else if(type==ge_common_struct::condition_type::UNEQUAL)
        {
            return db_value!=exp->cond.attribute_value;
        }
        else
        {
            //TODO ERROR
            return true;
        }
    }
    else
    {
        ge_common_struct::exp_node_type node_type=exp->type;
        if(node_type==ge_common_struct::exp_node_type::AND)
        {
            if(exp->left && exp->right)
            {
                return (ConditionCheck(exp->left) && ConditionCheck(exp->right));
            }
            else
            {
                //TODO error
                return true;
            }

        }
        else if(node_type==ge_common_struct::exp_node_type::OR)
        {

            if(exp->left && exp->right)
            {
                return (ConditionCheck(exp->left) || ConditionCheck(exp->right));
            }
            else
            {
                //TODO error
                return true;
            }

        }
        else
        {
            //TODO error
            return true;
        }
    }
}

bool CGameUISystem::ConditionCheck(ge_common_struct::dialog_tree_node* node)
{
    ge_common_struct::exp_node exp=node->expression;
    if(exp.type==ge_common_struct::exp_node_type::NO_CONDITION)
    {
        return true;
    }
    else
    {
        return ConditionCheck(&exp);
    }
}

void CGameUISystem::LoadNextLine()
{
    m_dialog.HideChoice();
    size_t cnt=m_cursor_node->node_text.size();
    if(m_line_no<cnt)
    {
        std::string text=m_cursor_node->node_text[m_line_no];
        m_dialog.SetText(text);
        m_line_no++;
    }
    else
    {
        size_t childcnt=m_cursor_node->children.size();
        if(childcnt==0)
        {
            //hide dialog;
            m_dialog.Hide();
        }
        else if(childcnt>0)
        {
            if(m_cursor_node->has_options)
            {

                m_dialog.ShowChoice();
                ge_common_struct::StringList choices;
                for(size_t i=0; i<m_cursor_node->children.size(); i++)
                {
                    std::string  opname=m_cursor_node->children[i]->option_name;
                    choices.push_back(opname);
                }
                m_dialog.initChoice(choices);

            }
            else
            {
                bool found=false;
                for(size_t i=0; i<m_cursor_node->children.size(); i++)
                {
                    ge_common_struct::dialog_tree_node* child_node=
                        m_cursor_node->children[i];
                    if(ConditionCheck(child_node))
                    {
                        m_cursor_node=child_node;
                        m_line_no=0;
                        found=true;
                        break;
                    }
                }
                if(found)
                {
                    LoadNextLine();
                }
                else
                {
                    m_dialog.Hide();
                }

            }
        }
    }
}

void CGameUISystem::ProcessInput(CInputEvent event)
{
    ge_common_struct::key_event kevent=event.GetInput().get_top_event();
    ge_common_struct::key_event_type event_type=kevent.key;
    ge_common_struct::key_press_type press_type=kevent.press;
    int state=event.GetCurrentSubState();
    if(press_type==ge_common_struct::key_press_type::KEY_PRESS)
    {
        if(state==1)
        {
            if(m_dialog.IsShow())
            {
                if(m_dialog.IsChoiceActive())
                {
                    if(event_type==ge_common_struct::key_event_type::KEY_DOWN)
                    {
                        m_dialog.GetChoiceDialog()->ChoiceDown();
                    }
                    else if(event_type==ge_common_struct::key_event_type::KEY_UP)
                    {
                        m_dialog.GetChoiceDialog()->ChoiceUp();
                    }
                    else if(event_type==ge_common_struct
                            ::key_event_type::KEY_SPACE
                            || event_type==ge_common_struct
                            ::key_event_type::KEY_ENTER)
                    {
                        SelChoice();

                    }
                }
                else
                {
                    LoadNextLine();
                }

            }
            else
            {
                if(m_menu_stack.size()>0)
                {
                    std::string menu_id=m_menu_stack.back();
                    if(m_panels.find(menu_id)!=m_panels.end())
                    {
                        ge_common_struct::dom_node* node=m_panels[menu_id];
                        //int cnt=node->children.size();
                        int cnt=ge_common_struct::CntDomChild(node);
                        //int row=node.row;
                        int col=node->col;
                        ge_common_struct::ui_layout layout=node->child_layout;
                        if(event_type==ge_common_struct::key_event_type::KEY_CANCLE)
                        {
                            m_menu_stack.pop_back();
                            if(m_elp_stack.size()>0){
                                m_el_pointer=m_elp_stack.back();
                                m_elp_stack.pop_back();
                            }
                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_DOWN)
                        {
                            if(layout==ge_common_struct::ui_layout::GRID_LAYOUT)
                            {
                                m_el_pointer+=col;
                            }
                            else
                            {
                                m_el_pointer++;
                            }
                            m_el_pointer=m_el_pointer%cnt;
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
                            m_el_pointer=(m_el_pointer+cnt)%cnt;

                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_LEFT)
                        {
                            m_el_pointer--;
                            m_el_pointer=(m_el_pointer+cnt)%cnt;
                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_RIGHT)
                        {
                            m_el_pointer++;
                            m_el_pointer=m_el_pointer%cnt;
                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_CONFIRM)
                        {
                            ge_common_struct::dom_node* sel
                            =ge_common_struct::GetDomSelection(node,m_el_pointer);
                            IMenuProcess * process=m_ui_manager.GetMenuInterface(menu_id);
                            if(process)
                            {
                                int obj_id=-1;
                                if(sel!=nullptr){
                                    obj_id=sel->obj_id;
                                }
                                process->Choose(obj_id,m_el_pointer);
                            }
                            //GE_LOG("%s\n",sel->action_name.c_str());
                            if(m_panels.find(sel->action_name)!=m_panels.end()){
                                m_menu_stack.push_back(sel->action_name);
                                ge_common_struct::dom_node* menu=m_panels[sel->action_name];
                                UpdateDomContent(menu);
                                m_elp_stack.push_back(m_el_pointer);
                                m_el_pointer=0;

                            }
                        }
                    }
                }

            }
        }
        else if(state==0)
        {
            if(event_type==ge_common_struct::key_event_type::KEY_CONFIRM)
            {
                m_el_pointer=0;
                m_menu_stack.push_back(m_confirm_menu);
                ge_common_struct::dom_node* menu=m_panels[m_confirm_menu];
                UpdateDomContent(menu);
            }
        }

    }

}

void CGameUISystem::SetDialogTree(ge_common_struct::dialog_tree_node* tree)
{
    m_dialog_tree=tree;
    m_cursor_node=tree;
    m_line_no=0;
    LoadNextLine();
}

void CGameUISystem::UpdateDomContent(ge_common_struct::dom_node* node
                                     ,int context_obj)
{
    if(node->list_template!=nullptr)
    {
        //当前节点是一个列表
        //node->children.clear();
        ge_common_struct::FreeDomVector(node->children);
        //node->children.clear();
        ge_common_struct::dom_node* list_template=node->list_template;
        std::vector<ge_common_struct::dom_node*>& children=
            list_template->children;
        std::string list_name=node->list_template->list_name;
        std::vector<int> ids=m_database->GetListObjectIds(list_name);
        for(size_t j=0; j<ids.size(); j++)
        {
            int id=ids[j];
            ge_common_struct::dom_node* list_item=
                new ge_common_struct::dom_node();
            node->children.push_back(list_item);
            list_item->ele_name="list_container";
            list_item->obj_id=id;
            list_item->child_seq_no=j;
            list_item->child_layout=ge_common_struct::
                                    ui_layout::HORIZONTAL_LAYOUT;
            list_item->style.border_color.a=0;
            list_item->style.background_color.a=0;
            list_item->parent_node=node;
            for(size_t i=0; i<children.size(); i++)
            {
                ge_common_struct::dom_node* temp=children[i];
                ge_common_struct::dom_node* child=CreateDomNode(temp,list_item,id);
                list_item->children.push_back(child);
            }
        }
    }
    if(node->obj_id>=0)
    {
        context_obj=node->obj_id;
    }
    if(node->children.size()>0)
    {

        for(size_t i=0; i<node->children.size(); i++)
        {
            UpdateDomContent(node->children[i],context_obj);
        }
    }
    else
    {
        if(node->use_template && context_obj>=0)
        {
            //TODO
            std::string template_text=node->template_text;
            //暂时写简单点
            size_t start_pos=template_text.find("{");
            size_t end_pos=template_text.find("}");
            std::string prop_name=template_text.substr(start_pos+1,end_pos-1);
            std::string obj_type=m_database->GetObjectType(context_obj);
            CGameDatabase::DataType type=m_database->GetPropType(obj_type,prop_name);
            if(prop_name.compare("label")==0)
            {
                std::string label=m_database->GetObjectLabel(context_obj);
                node->text=label;
            }
            else if(type==CGameDatabase::DataType::INTEGER)
            {
                int value=m_database->GetObjectData(context_obj,prop_name);
                node->text=""+value;
            }
            else if(type==CGameDatabase::DataType::TEXT)
            {
                std::string value=m_database->GetObjectText(context_obj,prop_name);
                node->text=value;
            }
            else if(type==CGameDatabase::DataType::ICON_ID)
            {
                std::string value=m_database->GetObjectText(context_obj,prop_name);
                node->text=value;
            }
        }
    }

}

ge_common_struct::dom_node* CGameUISystem::CreateDomNode(
    ge_common_struct::dom_node* temp,ge_common_struct::dom_node* parent,
    int context_id)
{
    ge_common_struct::dom_node* node=new ge_common_struct::dom_node();
    node->attributes=temp->attributes;
    node->box=temp->box;
    node->child_layout=temp->child_layout;
    node->col=temp->col;
    node->ele_name=temp->ele_name;
    node->list_name=temp->list_name;
    node->obj_id=context_id;
    node->parent_node=temp->parent_node;
    node->row=temp->row;
    node->style=temp->style;
    node->template_text=temp->template_text;
    node->use_template=temp->use_template;
    node->text=temp->text;
    if(temp->children.size()>0)
    {
        for(size_t i=0; i<temp->children.size(); i++)
        {
            ge_common_struct::dom_node* childtemp=temp->children[i];
            ge_common_struct::dom_node* child=CreateDomNode(childtemp,
                                              node,context_id);
            node->children.push_back(child);
        }
    }
    return node;
}

void CGameUISystem::LoadActions()
{
    CEquipMenu* equip_menu=new CEquipMenu();
    m_ui_manager.Register("equip_menu",equip_menu);
}
