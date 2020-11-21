#ifndef CGAMEUISYSTEM_H
#define CGAMEUISYSTEM_H

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
    protected:
        CGameContext* m_context;
        CAdvDialog  m_dialog;
        ge_common_struct::dialog_tree_node* m_dialog_tree;
        void InitDialog();
    private:
};

#endif // CGAMEUISYSTEM_H
