#ifndef CLAYER_H
#define CLAYER_H
#include "common_struct.h"

class CLayer
{
    public:
        CLayer();
        virtual ~CLayer();
        ge_common_struct::layer_type GetLayerType(){return m_type;};
        void SetLayerType(ge_common_struct::layer_type type){m_type=type;};
    protected:
        ge_common_struct::layer_type m_type;

    private:
};

#endif // CLAYER_H
