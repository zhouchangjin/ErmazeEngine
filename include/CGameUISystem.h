#ifndef CGAMEUISYSTEM_H
#define CGAMEUISYSTEM_H

#include "XML_Utilities.h"
#include "GameFileLoader.h"
#include <CGameDatabase.h>
#include <CServiceLocator.h>
#include <CAdvDialog.h>
#include <CSdlGameContext.h>
#include <IDefaultEventProcess.h>

class CGameUISystem :public IDefaultEventProcess
{
    public:
        CGameUISystem();
        CGameUISystem(CGameContext* context);
        virtual ~CGameUISystem();
        void SetContext(CGameContext* context){m_context=context;};
        void ProcessInput(CInputEvent event);
        void SetDialogTree(ge_common_struct::dialog_tree_node* tree);
        void Draw();
        void ShowDialog(){m_dialog.Show();};
        bool EventLock();
        void LoadUI();
        void LoadDatabase();
        void SetDialogStyle(ge_common_struct::dialog_style_node style);
    protected:
        CGameDatabase* m_database=nullptr;
        CGameContext* m_context;
        CAdvDialog  m_dialog;
        ge_common_struct::dialog_tree_node* m_dialog_tree=nullptr;
        ge_common_struct::dialog_tree_node* m_cursor_node=nullptr;
        ge_common_struct::dialog_style_node m_dialog_style;
        unsigned int m_line_no;
        unsigned int m_el_pointer=0;

        std::vector<std::string> m_menu_stack;

        std::vector<std::string> m_hud_stack;

        std::map<std::string,ge_common_struct::dom_node> m_panels;


        void UpdateDialogStyle();
        void LoadNextLine();
        void SelChoice();
        bool ConditionCheck(ge_common_struct::dialog_tree_node* node);
        bool ConditionCheck(ge_common_struct::exp_node* exp);
    private:
};

#endif // CGAMEUISYSTEM_H
