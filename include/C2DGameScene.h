#ifndef C2DGAMESCENE_H
#define C2DGAMESCENE_H

#include <CGameScene.h>
#include <CTileLayer.h>

class C2DGameScene :public CGameScene
{
    public:
        C2DGameScene();
        C2DGameScene(const C2DGameScene& copyscene);
        virtual ~C2DGameScene();

        const std::string GetTileCollideProp()const{return m_collide_prop;};
        const std::string GetLayerSwitchProp()const{return m_layerswitch_prop;};
        const std::string GetTileMapPath()const;
        void SetTileMapPath(std::string path){m_tilemap_path=path;};
        void SetCollideProp(std::string prop_name){m_collide_prop=prop_name;};
        void SetLayerSwitchProp(std::string prop_name){m_layerswitch_prop=prop_name;};

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


    protected:
        std::string m_tilemap_path;
        std::string m_collide_prop;
        std::string m_layerswitch_prop;

        int m_tile_width;
        int m_tile_height;
        int m_map_width;
        int m_map_height;
        void* m_tileset_texture;
        std::map<int,bool> m_collidable_tiles;
        std::map<int,bool> m_layerswitches;



    private:
};

#endif // C2DGAMESCENE_H
