#ifndef CRPGGAMEDATA_H
#define CRPGGAMEDATA_H

#include <string>
#include <map>

#include <GameFileLoader.h>
#include <XML_Utilities.h>
#include <Str_Utilities.h>
#include <CGameData.h>
#include <CSceneData.h>
#include <CSprite.h>

class CRPGGameData : public CGameData
{
    public:
        CRPGGameData();
        virtual ~CRPGGameData();

        void AddGameScene(std::string name,CSceneData scene);
        CSceneData GetGameScene(std::string name);
        void SetCurrentScene(std::string current_scene);
        CSceneData GetCurrentScene();
        void ParseGameDataByXMLDoc(xmlutils::MyXMLDoc* document);
        void AddSpriteSheet(std::string id,std::string path,int width,
                            int height,int col,int row);
        CSpriteSheet* GetSpriteSheet(std::string id);
        void AddPlayer(std::string id);
        CSprite* GetPlayer(int id);
        int GetPlayerOneCnt();
        void ClearPlayer();
        void AddSprite(std::string id,std::string sheet_id);
        void AddAction(std::string id,std::string action_name
                       ,std::vector<int>ids);
        void SetStartPoint(ge_common_struct::start_point2d startpoint);
        int GetStartX(){return m_startpoint.x;};
        int GetStartY(){return m_startpoint.y;};
        int GetStartLayer(){return m_startpoint.layer;};
        std::string GetStartScene(){return m_startpoint.start_scene;};
        std::string GetStartDirection(){return m_startpoint.direction;};
        ge_common_struct::dialog_style_node GetDialogStyle(){return m_dialog_style;};
    protected:

    private:
        std::string m_current_scene;
        std::map<std::string,CSceneData> m_scene_list;
        std::map<std::string,CSpriteSheet*> m_sprite_sheets;
        std::map<std::string,CSprite*> m_sprites;
        std::vector<std::string> m_player_one;
        ge_common_struct::start_point2d m_startpoint;
        ge_common_struct::dialog_style_node m_dialog_style;

};

#endif // CRPGGAMEDATA_H
