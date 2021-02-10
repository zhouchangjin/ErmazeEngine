#include "CTexture.h"

CTexture::CTexture(CTiledTexture* texture,int idx)
{
    m_texture=texture;
    m_idx=idx;
}

CTexture::~CTexture()
{
    //dtor
}
