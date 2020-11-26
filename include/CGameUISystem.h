#ifndef CGAMEUISYSTEM_H
#define CGAMEUISYSTEM_H

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
        void LoadDatabase();
    protected:
        CGameDatabase* m_database=nullptr;
        CGameContext* m_context;
        CAdvDialog  m_dialog;
        ge_common_struct::dialog_tree_node* m_dialog_tree=nullptr;
        ge_common_struct::dialog_tree_node* m_cursor_node=nullptr;
        size_t m_line_no;
        void InitDialog();
        void LoadNextLine();
        void SelChoice();
        bool ConditionCheck(ge_common_struct::dialog_tree_node* node);
        bool ConditionCheck(ge_common_struct::exp_node* exp);
    private:
};

#endif // CGAMEUISYSTEM_H