#ifndef CTILEDICON_H
#define CTILEDICON_H

#include <CSprite.h>


class CTiledIcon : public CSprite
{
    public:
        CTiledIcon();
        CTiledIcon(CSpriteSheet* sheet):CSprite(sheet){};
        virtual ~CTiledIcon();
        void AddIcon(std::string icon_name,int icon_idx);
        int GetIconPos(std::string icon_name);
    protected:

    private:
};

#endif // CTILEDICON_H
