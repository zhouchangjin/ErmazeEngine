#ifndef C2DGAMESCENE_H
#define C2DGAMESCENE_H

#include <CGameScene.h>
#include <CTileLayer.h>
#include <CSpriteSheet.h>
#include <CSceneData.h>
#include <CCamera2D.h>
#include <CSpriteGameObject.h>
#include <CTransferArea.h>
#include <CNPCGameObject.h>

class C2DGameScene :public CGameScene
{
    public:
        C2DGameScene();
        C2DGameScene(const C2DGameScene& copyscene);
        virtual ~C2DGameScene();
        void ClearScene();
        int GetTileWidth(){return m_tile_width;};
        int GetTileHeight(){return m_tile_height;};
        int GetMapWidth(){return m_map_width;};
        int GetMapHeight(){return m_map_height;};
        void SetTileWidth(int tile_width){m_tile_width=tile_width;};
        void SetTileHeight(int tile_height){m_tile_height=tile_height;};
        void SetMapWidth(int map_width){m_map_width=map_width;};
        void SetMapHeight(int map_height){m_map_height=map_height;};

        void SetTexture(void* texture){m_tileset_texture=texture;};
        void* GetTexture(){return m_tileset_texture;};

        void AddCollideTile(int tile_id);
        void AddSwitchTile(int tile_id);
        bool GetTileCollideBool(int tile_id);
        bool GetSwitchTileBool(int tile_id);
        CTileLayer* GetTileLayer(int layer_no);
        int GetCamera2DX(){return m_camera.GetCamera2DX();};
        int GetCamera2DY(){return m_camera.GetCamera2DY();};
        CCamera2D* GetCameraPointer(){return &m_camera;};
        void AddGridType(int gridx,int gridy,ge_common_struct::grid_type type);
        ge_common_struct::grid_type GetGridType(int gridx,int gridy);
        void AddTransferArea(CTransferArea area){m_transfer_area.push_back(area);};
        CTransferArea GetTransferArea(int i){return m_transfer_area[i];};
        int GetTransferAreaCnt(){return m_transfer_area.size();};
        void AddNpc(CNPCGameObject* npc){m_npcs.push_back(npc);};
        CNPCGameObject* GetNpc(int i){return m_npcs[i];};
        int GetNpcCnt(){return m_npcs.size();};
        void AddSprite(std::string id,CSpriteSheet* psheet);
        CSprite* GetSprite(std::string sprite_id);
        void AddSpriteAction(std::string id,std::string action_name,
                             std::vector<int> ids);
        CNPCGameObject* CreateNpc(std::string spr_id,int x,int y
                                  ,int layer,int direction);

        ge_common_struct::ge_point GetGridCoor(int x_coor,int y_coor);


    protected:

        int m_tile_width;
        int m_tile_height;
        int m_map_width;
        int m_map_height;
        void* m_tileset_texture;
        std::map<int,bool> m_collidable_tiles;
        std::map<int,bool> m_layerswitches;
        std::map<int,ge_common_struct::grid_type> m_grid_type;
        std::vector<CTransferArea> m_transfer_area;
        std::vector<CNPCGameObject*> m_npcs;
        std::map<std::string,CSprite*> m_sprites;
        CCamera2D m_camera;
    private:
};

#endif // C2DGAMESCENE_H
