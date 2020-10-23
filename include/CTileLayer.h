#ifndef CTILELAYER_H
#define CTILELAYER_H

#include <CLayer.h>

class CTileLayer : public CLayer
{
    public:
        CTileLayer();
        virtual ~CTileLayer();
        void AddRow(ge_common_struct::ROW_IDX row);
        ge_common_struct::LAYER_IDX GetTiles();

    protected:
        ge_common_struct::LAYER_IDX m_tiles;

    private:
};

#endif // CTILELAYER_H
