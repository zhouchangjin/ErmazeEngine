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
}

void CGameUISystem::SetDialogStyle(ge_common_struct::dialog_style_node style){
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

void CGameUISystem::LoadDatabase(){

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
        return false;
    }
}


void CGameUISystem::UpdateDialogStyle()
{
    ge_common_struct::ge_rect r=m_dialog_style.main_window.client_rect;
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    int dialog_x=0,dialog_y=0,dialog_w=0,dialog_h=0;
    if(m_dialog_style.main_window.is_percentage){
        dialog_x=fullWindow.x+(fullWindow.w*r.x/100);
        dialog_y=fullWindow.y+(fullWindow.h*r.y/100);
        dialog_w=fullWindow.w*r.w/100;
        dialog_h=fullWindow.h*r.h/100;
    }else{
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

void CGameUISystem::Draw()
{
    UpdateDialogStyle();
    if(m_dialog.IsShow())
    {
        sdlutil2::DrawAdvDialog(m_context,m_dialog);
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
    if(exp->left==nullptr){
       //is roof
       int db_value=-1;
       std::string attribute_name=exp->cond.attribute_name;
       size_t current=attribute_name.find(".");
       if(current!=std::string::npos){
         std::string obj_name=attribute_name.substr(0,current);
         std::string prop_name=attribute_name.substr(current+1);
         int obj_id=m_database->GetObjectId(obj_name);
         db_value=m_database->GetObjectData(obj_id,prop_name);
       }else{
         db_value=m_database->GetIntData(exp->cond.attribute_name);
       }
       ge_common_struct::condition_type type=exp->cond.type;
       if(type==ge_common_struct::condition_type::EQUAL){
            return db_value==exp->cond.attribute_value;
       }else if(type==ge_common_struct::condition_type::GREATER){
            return db_value>exp->cond.attribute_value;
       }else if(type==ge_common_struct::condition_type::LESS){
            return db_value<exp->cond.attribute_value;
       }else if(type==ge_common_struct::condition_type::EQUAL_GREATER){
            return db_value>=exp->cond.attribute_value;
       }else if(type==ge_common_struct::condition_type::EQUAL_LESS){
            return db_value<=exp->cond.attribute_value;
       }else if(type==ge_common_struct::condition_type::UNEQUAL){
            return db_value!=exp->cond.attribute_value;
       }else{
            //TODO ERROR
            return true;
       }
    }else{
        ge_common_struct::exp_node_type node_type=exp->type;
        if(node_type==ge_common_struct::exp_node_type::AND){
            if(exp->left && exp->right){
               return (ConditionCheck(exp->left) && ConditionCheck(exp->right));
            }else{
                //TODO error
                return true;
            }

        }else if(node_type==ge_common_struct::exp_node_type::OR){

            if(exp->left && exp->right){
               return (ConditionCheck(exp->left) || ConditionCheck(exp->right));
            }else{
                //TODO error
                return true;
            }

        }else{
            //TODO error
            return true;
        }
    }
}

bool CGameUISystem::ConditionCheck(ge_common_struct::dialog_tree_node* node)
{
    if(!m_database){
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
    ge_common_struct::input_event event_type=event.GetEventType();
    int state=event.GetCurrentSubState();
    if(state==1)
    {
        if(m_dialog.IsChoiceActive())
        {
            if(event_type==ge_common_struct::input_event::KEY_DOWN)
            {
                m_dialog.GetChoiceDialog()->ChoiceDown();
            }
            else if(event_type==ge_common_struct::input_event::KEY_UP)
            {
                m_dialog.GetChoiceDialog()->ChoiceUp();
            }
            else if(event_type==ge_common_struct::input_event::KEY_SPACE
                    || event_type==ge_common_struct::input_event::KEY_ENTER)
            {
                SelChoice();

            }
        }
        else
        {
            LoadNextLine();
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
