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
        int ActionStepCnt(std::string action_name);
        void SetSheet(CSpriteSheet* sheet){m_sprite_sheet=sheet;};
    protected:
        std::map<std::string,std::vector<int>> m_action_map;
    private:
        CSpriteSheet* m_sprite_sheet;

};

#endif // CSPRITE_H
