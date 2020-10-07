#ifndef CGAMESCENE_H
#define CGAMESCENE_H

#include <string>

class CGameScene
{
    public:
        CGameScene();
        CGameScene(const CGameScene& copyscene);
        virtual ~CGameScene();
        const std::string GetTileMapPath()const;
        void SetTileMapPath(std::string path){m_tilemap_path=path;};

    protected:

    private:
        std::string m_tilemap_path;

};

#endif // CGAMESCENE_H
