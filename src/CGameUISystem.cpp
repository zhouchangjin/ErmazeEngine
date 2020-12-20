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
    //dtor
    for(size_t i=0; i<m_spritesheets.size(); i++)
    {
        CSpriteSheet* sheet=m_spritesheets[i];
        delete sheet;
    }
    m_spritesheets.clear();
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
    xmlutils::MyXMLDoc icon_doc=xmlutils::LoadXML("./ui/resource.ui");
    xmlutils::MyXMLNode icon_sheet_node=icon_doc.
                                        GetNode("/ui/spritesheets");
    xmlutils::MyXMLNode icon_node=icon_doc.GetNode("/ui/icons");

    std::map<std::string,ge_common_struct::image_def> sheets;
    std::map<std::string,ge_common_struct::icon_def> icons;
    ge_fileutil::parse_sheets(icon_sheet_node,sheets,"./ui/res/");
    ge_fileutil::parse_icons(icon_node,icons);

    std::map<std::string,ge_common_struct::image_def>::iterator it;
    std::map<std::string,ge_common_struct::icon_def>::iterator it_icon;
    for(it=sheets.begin(); it!=sheets.end(); it++)
    {
        ge_common_struct::image_def image=it->second;
        CSpriteSheet* sprite_sheet=new CSpriteSheet(image.path,image.width,
                image.height,
                image.col,image.row);
        m_spritesheets.push_back(sprite_sheet);
        CTiledIcon tile_icon(sprite_sheet);
        m_icons[image.id]=tile_icon;
    }

    for(it_icon=icons.begin(); it_icon!=icons.end(); it_icon++)
    {
        ge_common_struct::icon_def icon=it_icon->second;
        std::string sheet_id=icon.resource_id;
        std::string icon_name=icon.icon_name;
        int icon_idx=icon.id;
        if(m_icons.find(sheet_id)!=m_icons.end())
        {
            m_icons[sheet_id].AddIcon(icon_name,icon_idx);
            m_icon_sheet_map[icon_name]=sheet_id;
        }
    }

    xmlutils::MyXMLDoc doc=xmlutils::LoadXML("./ui/menu.ui");
    xmlutils::MyXMLNode node=doc.GetNode("ui");

    xmlutils::MyXMLNode window=node.Child("window");
    int id=0;
    std::string idStr="";
    for(; window; window=window.NextSlibing("window"))
    {
        id++;
        ge_common_struct::dom_node node=ge_fileutil::parse_dom(window);
        if(window.HasAttribute("id"))
        {
            idStr=window.StrAttribute("id");
        }
        else
        {
            idStr+=std::to_string(id);
        }
        m_panels[idStr]=node;

    }

}

void CGameUISystem::LoadDatabase()
{

    m_database=CServiceLocator::
               GetService<CGameDatabase>
               (CServiceLocator::ServiceID::DATABASE);
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

CTiledIcon CGameUISystem::GetTileIcon(std::string icon_name)
{
    if(m_icon_sheet_map.find(icon_name)!=m_icon_sheet_map.end())
    {
        std::string sheet_id=m_icon_sheet_map[icon_name];
        CTiledIcon icon=m_icons[sheet_id];
        return icon;
    }
    else
    {
        return CTiledIcon(nullptr);
    }
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
            int offsetx=0;
            int offsety=0;
            std::string menu_id=m_menu_stack.back();
            if(m_panels.find(menu_id)!=m_panels.end())
            {
                ge_common_struct::dom_node menu=m_panels[menu_id];
                sdlutil2::UpdateDomRect(menu,fullWindow,offsetx,offsety);
                sdlutil2::DrawDomNode(m_context,menu);
                CTiledIcon icon=GetTileIcon(m_menu_pointer);
                sdlutil2::DrawIcon(m_context,menu,m_el_pointer,icon,m_menu_pointer,-10,0,m_icon_scale);
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
    if(!m_database)
    {
        LoadDatabase();
    }
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
                        ge_common_struct::dom_node node=m_panels[menu_id];
                        int cnt=node.children.size();
                        //int row=node.row;
                        int col=node.col;
                        ge_common_struct::ui_layout layout=node.child_layout;
                        if(event_type==ge_common_struct::key_event_type::KEY_CANCLE)
                        {
                            m_menu_stack.pop_back();
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
                        else if(event_type==ge_common_struct::key_event_type::KEY_LEFT){
                            m_el_pointer--;
                            m_el_pointer=(m_el_pointer+cnt)%cnt;
                        }
                        else if(event_type==ge_common_struct::key_event_type::KEY_RIGHT){
                            m_el_pointer++;
                            m_el_pointer=m_el_pointer%cnt;
                        }
                    }
                }

            }
        }
        else if(state==0)
        {
            if(event_type==ge_common_struct::key_event_type::KEY_CONFIRM)
            {
                m_menu_stack.push_back(m_confirm_menu);
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
