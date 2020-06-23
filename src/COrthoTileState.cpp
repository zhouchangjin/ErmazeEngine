#include "COrthoTileState.h"

COrthoTileState::COrthoTileState(){


}

COrthoTileState::COrthoTileState(CGameContext* context):CGameState(context)
{
    //ctor
}

COrthoTileState::~COrthoTileState()
{
    //dtor
    delete this->m_sprite_sheet;
    m_sprite_sheet=NULL;
    delete this->m_protagnist;
    m_protagnist=NULL;
}

void COrthoTileState::Init(){
    this->m_sprite_sheet=new CSpriteSheet();
    this->m_state_value=0;
    std::string path=this->m_sprite_sheet->GetSpritePath();

    //因为没有用依赖注入 当不使用SDL时该位置就不得不变
    void* sprite_texture=sdlutil2::LoadPngTexture(path,m_context);

    m_sprite_sheet->SetTexture(sprite_texture);
    std::vector<int> sequence={0,1,2,3,4,5,6,7};
    m_protagnist=new CSprite(m_sprite_sheet,sequence);
    m_protagnist->AddAction("downward",{0,1});
    m_protagnist->AddAction("upward",{2,3});
    m_protagnist->AddAction("left",{4,5});
    m_protagnist->AddAction("right",{6,7});
}

void COrthoTileState::Cleanup(){

}

void COrthoTileState::Draw(){

    //CRPGGameData* gamedata=(CRPGGameData*)this->m_game_data;
    //int x=gamedata->GetX();
    //int y=gamedata->GetY();
    ge_common_struct::ge_rect fullWindow=sdlutil2::LoadWindowRect(m_context);

    int centerx=fullWindow.w/2;
    int centery=fullWindow.h/2;

    sdlutil2::FillRect(m_context,fullWindow,0,0,0);
    int idx=m_protagnist->PlayAction(m_current_action,m_step);
    sdlutil2::RenderSprite(m_context,m_protagnist,idx,centerx,centery,2);
    sdlutil2::RenderPresent(m_context);

}

int COrthoTileState::GetStateValue(){
   return this->m_state_value;
}

void COrthoTileState::HandleEvent(ge_common_struct::game_event event){

    if(event==ge_common_struct::KEY_ESC){
        this->m_state_value=1;
    }else if(event==ge_common_struct::KEY_UP){
        m_step++;
        m_current_action="upward";
    }else if(event==ge_common_struct::KEY_DOWN){
        m_step++;
        m_current_action="downward";
    }else if(event==ge_common_struct::KEY_LEFT){
        m_step++;
        m_current_action="left";
    }else if(event==ge_common_struct::KEY_RIGHT){
        m_step++;
        m_current_action="right";
    }

}

void COrthoTileState::Pause(){

}

void COrthoTileState::Resume(){
    this->m_state_value=0;


}

void COrthoTileState::Update(){

}
