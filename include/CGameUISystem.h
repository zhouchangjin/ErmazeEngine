#ifndef CGAMEUISYSTEM_H
#define CGAMEUISYSTEM_H

#include "XML_Utilities.h"
#include "GameFileLoader.h"
#include <CGameDatabase.h>
#include <CServiceLocator.h>
#include <CAdvDialog.h>
#include <CSdlGameContext.h>
#include <IDefaultEventProcess.h>
#include <CImageDB.h>
#include <CUIActionManager.h>
#include <CEquipMenu.h>

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
        CGameDatabase* GetDatabase(){return m_database;};
        void LoadActions();
        void SetDialogStyle(ge_common_struct::dialog_style_node style);
    protected:
        CUIActionManager m_ui_manager;
        CGameContext* m_context;
        CGameDatabase* m_database=nullptr;
        CImageDB* m_imagedb=nullptr;
        CAdvDialog  m_dialog;

        ge_common_struct::dialog_tree_node* m_dialog_tree=nullptr;
        ge_common_struct::dialog_tree_node* m_cursor_node=nullptr;
        ge_common_struct::dialog_style_node m_dialog_style;

        unsigned int m_line_no;
        int m_el_pointer=0;

        //HARD CODING
        std::string m_confirm_menu="guide_menu";//"equip_menu";//"guide_menu";
        std::string m_menu_pointer="point_right";
        int m_icon_scale=2;

        std::vector<std::string> m_menu_stack;
        std::vector<std::string> m_hud_stack;
        std::vector<int> m_elp_stack;


        std::map<std::string,ge_common_struct::dom_node*> m_panels;

        CTiledTexture GetTileIcon(std::string icon_name);
        void UpdateDialogStyle();
        void LoadNextLine();
        void SelChoice();
        bool ConditionCheck(ge_common_struct::dialog_tree_node* node);
        bool ConditionCheck(ge_common_struct::exp_node* exp);
        void UpdateDomContent(ge_common_struct::dom_node* node,int context_obj=-1);
        ge_common_struct::dom_node* CreateDomNode(ge_common_struct::dom_node* temp,ge_common_struct::dom_node* parent,int context_obj);
    private:
};

#endif // CGAMEUISYSTEM_H
