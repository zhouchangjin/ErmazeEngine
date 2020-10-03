#include "CSprite.h"

CSprite::CSprite()
{
    //ctor
}

CSprite::CSprite(CSpriteSheet* sheet,std::vector<int> sequence){
    m_sprite_sheet=sheet;
    m_sequence.swap(sequence);
}

CSprite::~CSprite()
{
    //dtor
}


ge_common_struct::ge_rect CSprite::GetRectByIdx(int sprite_idx){

    int spritesheet_idx=m_sequence[sprite_idx];
    return m_sprite_sheet->GetRectByIdx(spritesheet_idx);
}

void CSprite::AddAction(std::string action_name,std::vector<int> frames){
    //std::vector<int>* action_frames=new std::vector<int>();
    //action_frames->swap(frames);
    m_action_map[action_name]=frames;//action_frames;
}

int CSprite::PlayAction(std::string action_name,int step){

   std::vector<int> frames= m_action_map[action_name];
   uint32_t frames_cnt=frames.size();
   int frameidx=frames[step%frames_cnt];

   return frameidx;
}
