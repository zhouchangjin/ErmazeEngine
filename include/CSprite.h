#ifndef CSPRITE_H
#define CSPRITE_H

#include "CSpriteSheet.h"

class CSprite
{
    public:
        CSprite();
        CSprite(CSpriteSheet* sprite_sheet);
        virtual ~CSprite();
        CSpriteSheet* GetSpriteSheet(){return this->m_sprite_sheet;};
        ge_common_struct::ge_rect GetRectByIdx(int sprite_idx);
        void AddAction(std::string action_name,std::vector<int> frame_idx_arr);
        int PlayAction(std::string action_name,int step);
        void SetSheet(CSpriteSheet* sheet){m_sprite_sheet=sheet;};
    protected:

    private:
        int m_frame_idx=0;
        CSpriteSheet* m_sprite_sheet;
        std::map<std::string,std::vector<int>> m_action_map;
};

#endif // CSPRITE_H
