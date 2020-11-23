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


void CGameUISystem::InitDialog()
{
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);
    m_dialog.SetX(fullWindow.x);
    m_dialog.SetY(fullWindow.h*2/3);
    m_dialog.SetWidth(fullWindow.w);
    m_dialog.SetHeight(fullWindow.h/3);
    m_dialog.SetBackGroundColor(0,0,0,200);
    m_dialog.SetTitleHeight(50);
    m_dialog.SetTitleWidth(200);
    m_dialog.SetTitleX(fullWindow.x);
    m_dialog.SetTitleY(m_dialog.GetY()-m_dialog.GetTitleHeight()
                       +m_dialog.GetBorderWidth());
    m_dialog.SetFontColor(255,255,255);
    m_dialog.InitResponseByDialog();
    m_dialog.SetDisplaySpeed(3);
    m_dialog.ShowIndicator();//永远显示提示指针比较方便
}

void CGameUISystem::Draw()
{
    InitDialog();
    if(m_dialog.IsShow())
    {
        sdlutil2::DrawAdvDialog(m_context,m_dialog);
    }
}

void CGameUISystem::SelChoice(){
    size_t sel_id=m_dialog.GetChoiceDialog()->GetSelection();
    m_cursor_node=m_cursor_node->children[sel_id];
    m_line_no=0;
    LoadNextLine();
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
        else if(childcnt==1)
        {
            m_cursor_node=m_cursor_node->children[0];
            m_line_no=0;
            LoadNextLine();
        }
        else if(childcnt>1)
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
