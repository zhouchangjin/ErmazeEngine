#ifndef CSPRITESHEET_H
#define CSPRITESHEET_H
#include <string>
#include <vector>
#include <map>
#include "common_struct.h"

class CSpriteSheet
{
    public:
        CSpriteSheet();
        virtual ~CSpriteSheet();
        void* GetTexture(){return m_texture;};
        std::string GetSpritePath(){return m_sprite_path;};
        void SetTexture(void* pointer){m_texture=pointer;};
        ge_common_struct::ge_rect GetRectByIdx(int index);
    protected:

    private:
        std::string m_sprite_path="res/sprites.png";
        void * m_texture=NULL;
        int m_sprite_width=16;
        int m_sprite_height=16;
        int m_resource_width=8;
        int m_resource_height=5;
};

#endif // CSPRITESHEET_H
