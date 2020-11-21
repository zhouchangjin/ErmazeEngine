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
    m_frame_idx=m_sprite->PlayAction(action_name,m_step);
}

void CSpriteGameObject::Play()
{
    m_frame_idx=m_sprite->PlayAction(m_current_action,m_step);
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
    }

}

void CSpriteGameObject::UpdateXY(int x,int y)
{
    if(x!=m_x)
    {
        m_x=x;
    }
    if(y!=m_y)
    {
        m_y=y;
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
void CSpriteGameObject::UpdateShowLayer(int layer)
{
    if(layer>=0)
    {
        m_show_layer=layer;
    }
}
void CSpriteGameObject::UpdateLayer(int layer)
{
    if(layer>=0 && m_layer!=layer)
    {
        m_layer=layer;
        m_show_layer=layer;
    }
}

int CSpriteGameObject::GetShowLayer()
{
    if(m_onstair)
    {
        return m_show_layer;
    }
    else
    {
        return m_layer;
    }
}

void CSpriteGameObject::UpdateDirection(std::string action_name)
{
    m_current_action=action_name;
}

void CSpriteGameObject::BindPal(CSpriteGameObject* pal)
{
    m_pal=pal;
}

void CSpriteGameObject::MoveUpward(int dy)
{
    if(m_move_x==0 && m_move_y==0)
    {
        m_move_y=-1*dy;
    }
    UpdateDirection("upward");
}

void CSpriteGameObject::MoveDownward(int dy)
{
    if(m_move_x==0 && m_move_y==0)
    {
        m_move_y=dy;
    }
    UpdateDirection("downward");
}

void CSpriteGameObject::MoveLeftward(int dx)
{
    if(m_move_x==0 && m_move_y==0)
    {
        m_move_x=-1*dx;
    }
    UpdateDirection("leftward");
}

void CSpriteGameObject::MoveRightward(int dx)
{
    if(m_move_x==0 && m_move_y==0)
    {
        m_move_x=dx;
    }
    UpdateDirection("rightward");
}

void CSpriteGameObject::MoveUpward()
{
    m_move_y-=m_move_speed;
    UpdateDirection("upward");
}

void CSpriteGameObject::MoveDownward()
{
    m_move_y+=m_move_speed;
    UpdateDirection("downward");
}

void CSpriteGameObject::MoveLeftward()
{
    m_move_x-=m_move_speed;
    UpdateDirection("leftward");
}

void CSpriteGameObject::MoveRightward()
{
    m_move_x+=m_move_speed;
    UpdateDirection("rightward");
}

bool CSpriteGameObject::IsMoving()
{
    return (m_move_x!=0 || m_move_y!=0);
}

void CSpriteGameObject::StopMoving()
{
    m_move_x=0;
    m_move_y=0;
}

void CSpriteGameObject::MoveUpdate()
{
    ge_common_struct::action_type log=ge_common_struct::action_type::NO_MOVE;
    if(m_move_y<0)
    {

        log=ge_common_struct::action_type::MOVE_UP;
        if(m_move_y+m_move_speed>0)
        {
            m_move_y=0;
            UpdateY(m_y+m_move_y);
        }
        else
        {
            m_move_y+=m_move_speed;
            UpdateY(m_y-m_move_speed);
        }
        Step();
    }
    else if(m_move_y>0)
    {

        log=ge_common_struct::action_type::MOVE_DOWN;
        if(m_move_y-m_move_speed<0)
        {
            m_move_y=0;
            UpdateY(m_y+m_move_y);
        }
        else
        {
            m_move_y-=m_move_speed;
            UpdateY(m_y+m_move_speed);
        }
        Step();
    }
    else if(m_move_x<0)
    {

        log=ge_common_struct::action_type::MOVE_LEFT;
        if(m_move_x+m_move_speed>0)
        {
            m_move_x=0;
            UpdateX(m_x+m_move_x);
        }
        else
        {
            m_move_x+=m_move_speed;
            UpdateX(m_x-m_move_speed);
        }
        Step();
    }
    else if(m_move_x>0)
    {

        log=ge_common_struct::action_type::MOVE_RIGHT;
        if(m_move_x-m_move_speed<0)
        {
            m_move_x=0;
            UpdateX(m_x+m_move_x);
        }
        else
        {
            m_move_x-=m_move_speed;
            UpdateX(m_x+m_move_speed);
        }
        Step();
    }
    if(m_pal)
    {
        if(log!=ge_common_struct::action_type::NO_MOVE)
        {
            m_pal->AddActionLog(log);
        }
    }
    if(m_camera)
    {
        GE_LOG("player coor (%d,%d)\n",m_x,m_y);
    }
    Play();
}


void CSpriteGameObject::AddActionLog(ge_common_struct::action_type log)
{
    m_action_log.push(log);
    if(m_action_log.size()>m_pop_size)
    {
        ge_common_struct::action_type act=m_action_log.front();
        m_action_log.pop();
        if(act==ge_common_struct::action_type::MOVE_UP)
        {
            MoveUpward();
        }
        else if(act==ge_common_struct::action_type::MOVE_DOWN)
        {
            MoveDownward();
        }
        else if(act==ge_common_struct::action_type::MOVE_LEFT)
        {
            MoveLeftward();
        }
        else if(act==ge_common_struct::action_type::MOVE_RIGHT)
        {
            MoveRightward();
        }
    }
    MoveUpdate();
}

void CSpriteGameObject::ClearActionLog()
{
    while(!m_action_log.empty())
    {
        m_action_log.pop();
    }
}

bool CSpriteGameObject::CheckCollision(const CSpriteGameObject& obj)
{
    int layer=obj.GetLayer();
    if(m_layer!=layer)
    {
        return false;
    }
    else
    {
        int width=obj.GetObjectWidth();
        int height=obj.GetObjectHeight();
        int spd=obj.GetMoveSpeed();
        int movex=obj.GetMoveX();
        int movey=obj.GetMoveY();
        int obj_x=obj.GetX();
        int obj_y=obj.GetY();
        if(movex>0)
        {
            obj_x+=spd;
        }
        else if(movex<0)
        {
            obj_x-=spd;
        }
        if(movey<0)
        {
            obj_y-=spd;
        }
        else if(movey>0)
        {
            obj_y+=spd;
        }

        if(m_move_x>0)
        {
            int mright=m_x+m_move_speed+GetObjectWidth()-1;
            int mbottom=m_y+GetObjectHeight()-1;
            if(mright>=obj_x && mright<obj_x+width
                    && ((m_y>=obj_y && m_y<obj_y+height)||
                        (mbottom>=obj_y && mbottom<obj_y+height)))
            {
                return true;
            }
            else
            {
                return false;
            }

        }
        else if(m_move_x<0)
        {
            int mx=m_x-m_move_speed;
            int mbottom=m_y+GetObjectHeight()-1;
            if((mx>=obj_x && mx<obj_x+width)
                    && ((m_y>=obj_y && m_y<obj_y+height)||
                        (mbottom>=obj_y && mbottom<obj_y+height)))
            {

                return true;
            }
            else
            {
                return false;
            }

        }
        else if(m_move_y<0)
        {
            int mright=m_x+GetObjectWidth()-1;
            int mtop=m_y-m_move_speed;
            if( (mtop>=obj_y && mtop<obj_y+height)
                    && ((m_x>=obj_x && m_x<obj_x+width)
                        || (mright>=obj_x && mright<obj_x+width)))
            {
                return true;
            }else{
                return false;
            }

        }
        else if(m_move_y>0)
        {
            int mright=m_x+GetObjectWidth()-1;
            int mbottom=m_y+m_move_speed+GetObjectHeight()-1;

            if( (mbottom>=obj_y && mbottom<obj_y+height) &&
                ((m_x>=obj_x && m_x<obj_x+width)
                 ||(mright>=obj_x && mright<obj_x+width))){

                return true;
            }else{
                return false;
            }

        }else{
            return false;
        }
    }



}


int CSpriteGameObject::GetCurrentOrientation()const{
    if(m_current_action=="upward"){
        return ge_common_struct::action_source::FACE_UP;
    }else if(m_current_action=="downward"){
        return ge_common_struct::action_source::FACE_DOWN;
    }else if(m_current_action=="leftward"){
        return ge_common_struct::action_source::FACE_LEFT;
    }else if(m_current_action=="rightward"){
        return ge_common_struct::action_source::FACE_RIGHT;
    }else{
        return 0;
    }

}
