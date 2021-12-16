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
        CSpriteSheet(std::string sprite_path,int width,
                     int height,int col,int row);
        virtual ~CSpriteSheet();
        void* GetTexture(){return m_texture;};
        void* GetSurface(){return m_surface;};
        std::string GetSpritePath(){return m_sprite_path;};
        int GetSpriteWidth(){return m_sprite_width;};
        int GetSpriteHeight(){return m_sprite_height;};
        void SetTexture(void* pointer){m_texture=pointer;};
        void SetSurface(void* surface_pointer){m_surface=surface_pointer;};
        ge_common_struct::ge_rect GetRectByIdx(int index);

    protected:

    private:
        std::string m_sprite_path;
        void * m_texture=nullptr;
        void * m_surface=nullptr;
        int m_sprite_width=32;
        int m_sprite_height=32;
        int m_resource_width=8;
        int m_resource_height=5;
};

#endif // CSPRITESHEET_H
