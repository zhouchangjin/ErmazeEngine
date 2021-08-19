#ifndef CICON_H
#define CICON_H

#include<CTiledTexture.h>

class CTexture
{
    public:
        CTexture(CTiledTexture* texture,int idx);
        virtual ~CTexture();
        CTiledTexture* GetTiledTexture(){return m_texture;};
        int GetIdx(){return m_idx;};
        explicit operator bool();
    protected:
        CTiledTexture* m_texture;
        int m_idx;

    private:
};

#endif // CICON_H
