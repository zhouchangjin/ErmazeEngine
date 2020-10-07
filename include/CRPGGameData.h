#ifndef CRPGGAMEDATA_H
#define CRPGGAMEDATA_H

#include <string>
#include <map>

#include <CGameData.h>
#include <CGameScene.h>
#include <XML_Utilities.h>

class CRPGGameData : public CGameData
{
    public:
        CRPGGameData();
        virtual ~CRPGGameData();

        void AddGameScene(std::string name,CGameScene scene);
        CGameScene GetGameScene(std::string name);
        void SetCurrentScene(std::string current_scene);
        CGameScene GetCurrentScene();
        void ParseGameDataByXMLDoc(xmlutils::MyXMLDoc* document);


    protected:

    private:
        std::string m_current_scene;
        std::map<std::string,CGameScene> m_scene_list;


};

#endif // CRPGGAMEDATA_H
