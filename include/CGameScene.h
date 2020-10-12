#ifndef CGAMESCENE_H
#define CGAMESCENE_H

#include <map>
#include <string>
#include "common_struct.h"

class CGameScene
{
    public:
        CGameScene();
        CGameScene(const CGameScene& copyscene);
        virtual ~CGameScene();
        const std::string GetTileMapPath()const;
        void SetTileMapPath(std::string path){m_tilemap_path=path;};
        int GetLayerCnt(){return m_layers.size();};
        void AddLayer(ge_common_struct::LAYER_IDX layer){m_layers.push_back(layer);};
        ge_common_struct::LAYER_IDX GetLayer(int layer_no){return m_layers[layer_no];};
        void ClearScene(){m_layers.clear();};
        void SetTexture(void* texture){m_tileset_texture=texture;};
        void* GetTexture(){return m_tileset_texture;};
        int GetTileWidth(){return m_tile_width;};
        int GetTileHeight(){return m_tile_height;};
        int GetMapWidth(){return m_map_width;};
        int GetMapHeight(){return m_map_height;};
        std::string GetTileCollideProp(){return m_collide_prop;};
        bool GetTileCollideBool(int tile_id);
        void SetTileWidth(int tile_width){m_tile_width=tile_width;};
        void SetTileHeight(int tile_height){m_tile_height=tile_height;};
        void SetMapWidth(int map_width){m_map_width=map_width;};
        void SetMapHeight(int map_height){m_map_height=map_height;};
        void SetCollideProp(std::string prop_name){m_collide_prop=prop_name;};
        void AddCollideTile(int tile_id);

    protected:

    private:
        int m_tile_width;
        int m_tile_height;
        int m_map_width;
        int m_map_height;
        std::string m_tilemap_path;
        std::vector<ge_common_struct::LAYER_IDX> m_layers;
        void* m_tileset_texture;
        std::map<int,bool> m_collidable_tiles;
        std::string m_collide_prop;

};

#endif // CGAMESCENE_H
