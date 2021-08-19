#include "CSprite.h"

CSprite::CSprite()
{
    //ctor
}

CSprite::CSprite(CSpriteSheet* sheet){
    m_sprite_sheet=sheet;
}

CSprite::~CSprite()
{
    //dtor
}


ge_common_struct::ge_rect CSprite::GetRectByIdx(int sprite_idx){

    return m_sprite_sheet->GetRectByIdx(sprite_idx);
}

void CSprite::AddAction(std::string action_name,std::vector<int> frames){
    //std::vector<int>* action_frames=new std::vector<int>();
    //action_frames->swap(frames);
    if(frames.size()>0){
      m_action_map[action_name]=frames;
    }
}

int CSprite::ActionStepCnt(std::string action_name){
    if(m_action_map.find(action_name)!=m_action_map.end()){
        return m_action_map[action_name].size();
    }else{
        return 0;
    }
}

int CSprite::PlayAction(std::string action_name,int step){

   std::vector<int> frames= m_action_map[action_name];
   uint32_t frames_cnt=frames.size();
   int frameidx=frames[step%frames_cnt];
   return frameidx;
}
