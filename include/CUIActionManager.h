#ifndef CUIACTIONMANAGER_H
#define CUIACTIONMANAGER_H

#include<map>
#include<string>
#include<IMenuProcess.h>

class CUIActionManager
{
    public:
        CUIActionManager();
        virtual ~CUIActionManager();
        void Register(std::string name,IMenuProcess* instance);
        IMenuProcess* GetMenuInterface(std::string name);
    protected:
        std::map<std::string,IMenuProcess*> m_ui_action;
        std::map<std::string,std::string> m_panel_action;

    private:
};

#endif // CUIACTIONMANAGER_H
