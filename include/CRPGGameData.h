#ifndef CRPGGAMEDATA_H
#define CRPGGAMEDATA_H

#include <string>
#include <map>

#include <CGameData.h>
#include <C2DGameScene.h>
#include <XML_Utilities.h>

class CRPGGameData : public CGameData
{
    public:
        CRPGGameData();
        virtual ~CRPGGameData();

        void AddGameScene(std::string name,C2DGameScene scene);
        C2DGameScene GetGameScene(std::string name);
        void SetCurrentScene(std::string current_scene);
        C2DGameScene GetCurrentScene();
        void ParseGameDataByXMLDoc(xmlutils::MyXMLDoc* document);


    protected:

    private:
        std::string m_current_scene;
        std::map<std::string,C2DGameScene> m_scene_list;


};

#endif // CRPGGAMEDATA_H
