#include "CSpriteGameObject.h"

CSpriteGameObject::CSpriteGameObject(CSprite* sprite)
{
    m_sprite=sprite;
}

CSpriteGameObject::~CSpriteGameObject()
{

}

void CSpriteGameObject::PlayAction(std::string action_name)
{
    m_sprite->PlayAction(action_name,m_step);
}

void CSpriteGameObject::Play()
{
    m_sprite->PlayAction(m_current_action,m_step);
}

void CSpriteGameObject::UpdateX(int x)
{
    if(x!=m_x)
    {
        m_x=x;
        if(m_camera)
        {
            m_camera->SetCamera2DX(m_x);
        }
        if(m_pal)
        {
            m_pal->UpdateX(m_x);
        }
    }
}

void CSpriteGameObject::UpdateY(int y)
{
    if(y!=m_y)
    {
        m_y=y;
        if(m_camera)
        {
            m_camera->SetCamera2DY(y);
        }
        if(m_pal)
        {
            m_pal->UpdateY(m_y);
        }
    }

}

void CSpriteGameObject::UpdateXY(int x,int y)
{
    if(x!=m_x)
    {
        m_x=x;
    }
    if(y!=m_y){
       m_y=y;
    }
    if(m_pal){
        m_pal->UpdateXY(m_x,m_y);
    }
    if(m_camera)
    {
        m_camera->SetCamera2DX(m_x);
        m_camera->SetCamera2DY(m_y);
    }
}

void CSpriteGameObject::BindCamera(CCamera2D* camera)
{
    m_camera=camera;
    m_camera->SetCamera2DX(m_x);
    m_camera->SetCamera2DY(m_y);

}
void CSpriteGameObject::UpdateShowLayer(int layer){
    if(layer>=0){
        m_show_layer=layer;
    }
}
void CSpriteGameObject::UpdateLayer(int layer){
    if(layer>=0 && m_layer!=layer){
        m_layer=layer;
        m_show_layer=layer;
    }
}

int CSpriteGameObject::GetShowLayer(){
    if(m_onstair){
        return m_show_layer;
    }else{
        return m_layer;
    }
}

void CSpriteGameObject::UpdateDirection(std::string action_name)
{
    //compare string is wasting time
    m_current_action=action_name;
    m_step++;
    if(m_pal){
        m_pal->UpdateDirection(m_current_action);
    }
}

void CSpriteGameObject::BindPal(CSpriteGameObject* pal)
{
    m_pal=pal;
    m_pal->UpdateXY(m_x,m_y);
    m_pal->UpdateDirection(m_current_action);
}

void CSpriteGameObject::MoveUpward(){
    m_move_y-=m_move_speed;
}

void CSpriteGameObject::MoveDownward(){
    m_move_y+=m_move_speed;
}

void CSpriteGameObject::MoveLeftward(){
    m_move_x-=m_move_speed;
}

void CSpriteGameObject::MoveRightward(){
    m_move_x+=m_move_speed;
}

bool CSpriteGameObject::IsMoving(){
    return (m_move_x!=0 || m_move_y!=0);
}

void CSpriteGameObject::StopMoving(){
    m_move_x=0;
    m_move_y=0;
}

void CSpriteGameObject::MoveUpdate(){
    UpdateXY(m_x+m_move_x,m_y+m_move_y);
    if(m_move_y<0){
        UpdateDirection("upward");
    }else if(m_move_y>0){
        UpdateDirection("downward");
    }else if(m_move_x<0){
        UpdateDirection("leftward");
    }else if(m_move_x>0){
        UpdateDirection("rightward");
    }
    m_move_x=0;
    m_move_y=0;
    Play();
}
