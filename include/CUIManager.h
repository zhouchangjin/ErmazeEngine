#ifndef CUIMANAGER_H
#define CUIMANAGER_H

#include <Render_Util.h>
#include <CUIActionManager.h>
#include <CGameContext.h>
#include <CGameDatabase.h>
#include <CImageDB.h>
#include <CServiceLocator.h>
#include <IDefaultMenuEventProcess.h>
#include <CMenuInputEvent.h>

class CUIManager :public IDefaultMenuEventProcess
{
    public:
        enum UIState{
            POP_INIT,
            POP_MENU
        };
        CUIManager();
        CUIManager(CGameContext* context){m_context=context;};
        virtual ~CUIManager();
        void Init();
        void SetGameContext(CGameContext* context){m_context=context;};
        void AddPanel(std::string panel_name,ge_common_struct::dom_node* node,bool is_hide=false,bool is_pop=false);
        void Register(std::string name,IMenuProcess* instance);
        void SetPanelList(std::string panel_name,std::string list_name);
        void SetPanelObject(std::string panel_name,int obj_id);
        void ShowPopPanel(std::string panel_name);
        void SetMenuPointerName(std::string pointer_name){m_menu_pointer_name=pointer_name;};
        void CloseCurrentPopPanel();
        void HideAllPopPanel();
        void Draw();
        void Update();
        void ProcessInput(CMenuInputEvent event);
        bool IsPopPanelHidden();
        std::vector<ge_common_struct::menu_command> GetCommand(){return m_last_command;};
    protected:

        CUIActionManager m_action_manager;
        CGameContext* m_context;
        CGameDatabase* m_database=nullptr;
        CImageDB* m_imagedb=nullptr;


        int m_el_pointer=0;
        std::string m_menu_pointer_name;

        std::vector<std::string> m_pop_panel_stack;
        std::vector<int> m_elp_stack;
        std::vector<ge_common_struct::menu_command> m_current_command;
        std::vector<ge_common_struct::menu_command> m_last_command;
        std::vector<std::string> m_panel_list;
        std::vector<std::string> m_pop_panel_list;
        std::map<std::string,ge_common_struct::dom_node*> m_panels;
        std::map<std::string,int> m_panel_page_start;

    private:

};

#endif // CUIMANAGER_H
