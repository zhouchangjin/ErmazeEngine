#include "CParticle.h"

CParticle::CParticle()
{
    //ctor
}

CParticle::~CParticle()
{
    //dtor
}

CParticle::CParticle(int x,int y,int vx,int vy,int life)
{

    m_pos_x=x;
    m_pos_y=y;
    m_velocity_x=vx;
    m_velocity_y=vy;
    m_life=life;
}

void CParticle::AutoUpdate()
{
    if(m_life>=0){
       m_velocity_x+=m_acc_x;
        m_velocity_y+=m_acc_y;
        m_pos_x+=m_velocity_x;
        m_pos_y+=m_velocity_y;
        m_life--;
    }

}
