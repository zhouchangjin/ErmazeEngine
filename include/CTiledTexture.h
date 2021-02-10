#ifndef CTILEDICON_H
#define CTILEDICON_H

#include <CSprite.h>


class CTiledTexture : public CSprite
{
    public:
        CTiledTexture();
        CTiledTexture(CSpriteSheet* sheet):CSprite(sheet){};
        virtual ~CTiledTexture();
        void AddTexture(std::string texture_name,int texture_idx);
        int GetTexturePos(std::string texture_name);
    protected:

    private:
};

#endif // CTILEDICON_H
