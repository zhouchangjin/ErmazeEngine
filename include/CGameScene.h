#ifndef CGAMESCENE_H
#define CGAMESCENE_H

#include <map>
#include <string>

#include <CLayer.h>

class CGameScene
{
    public:
        CGameScene();
        CGameScene(const CGameScene& copyscene);
        virtual ~CGameScene();
        int GetLayerCnt();
        void ClearScene();
        CLayer* GetLayer(int layer_no);
        void AddLayer(CLayer* layer);
    protected:
        std::vector<CLayer*> m_layer_list;
    private:


};

#endif // CGAMESCENE_H
