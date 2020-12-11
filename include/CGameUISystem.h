#ifndef CGAMEUISYSTEM_H
#define CGAMEUISYSTEM_H

#include "XML_Utilities.h"
#include "GameFileLoader.h"
#include <CGameDatabase.h>
#include <CServiceLocator.h>
#include <CAdvDialog.h>
#include <CSdlGameContext.h>
#include <IDefaultEventProcess.h>
#include <CTiledIcon.h>

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
        CGameContext* m_context;
        CGameDatabase* m_database=nullptr;
        CAdvDialog  m_dialog;

        ge_common_struct::dialog_tree_node* m_dialog_tree=nullptr;
        ge_common_struct::dialog_tree_node* m_cursor_node=nullptr;
        ge_common_struct::dialog_style_node m_dialog_style;

        unsigned int m_line_no;
        unsigned int m_el_pointer=0;

        //HARD CODING
        std::string m_confirm_menu="guide_menu";
        std::string m_menu_pointer="point_right";
        int m_icon_scale=2;

        std::vector<std::string> m_menu_stack;
        std::vector<std::string> m_hud_stack;
        std::vector<CSpriteSheet*> m_spritesheets;
        std::map<std::string,CTiledIcon> m_icons;
        std::map<std::string,std::string> m_icon_sheet_map;
        std::map<std::string,ge_common_struct::dom_node> m_panels;

        CTiledIcon GetTileIcon(std::string icon_name);
        void UpdateDialogStyle();
        void LoadNextLine();
        void SelChoice();
        bool ConditionCheck(ge_common_struct::dialog_tree_node* node);
        bool ConditionCheck(ge_common_struct::exp_node* exp);
    private:
};

#endif // CGAMEUISYSTEM_H
