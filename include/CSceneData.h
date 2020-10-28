#ifndef CSCENEDATA_H
#define CSCENEDATA_H

#include <string>

class CSceneData
{
    public:
        CSceneData();
        virtual ~CSceneData();
        CSceneData(const CSceneData& copyscene);


        const std::string GetTileCollideProp()const{return m_collide_prop;};
        const std::string GetLayerSwitchProp()const{return m_layerswitch_prop;};
        const std::string GetTileMapPath()const;
        void SetTileMapPath(std::string path){m_tilemap_path=path;};
        void SetCollideProp(std::string prop_name){m_collide_prop=prop_name;};
        void SetLayerSwitchProp(std::string prop_name){m_layerswitch_prop=prop_name;};


    protected:
        std::string m_tilemap_path;
        std::string m_collide_prop;
        std::string m_layerswitch_prop;
    private:
};

#endif // CSCENEDATA_H
