#include "CBaseParticleEmitter.h"

CBaseParticleEmitter::CBaseParticleEmitter(CParticlePool* pool)
    :m_particle_pool{pool}
{

}

CBaseParticleEmitter::~CBaseParticleEmitter()
{
    //dtor
}

bool CBaseParticleEmitter::IsDead(){
   return m_emitter_life<0;
}

void CBaseParticleEmitter::Init()
{
    if(m_particle_pool!=nullptr)
    {
        for(int i=0; i<m_particle_cnt; i++)
        {
            CParticle* p=CreateParticle();
            InitParticle(p);
        }
    }
}

CParticle* CBaseParticleEmitter::CreateParticle(){
    if(m_particles.size()<m_max_particle_cnt){
        CParticle* p=m_particle_pool->GetOneIdleParitcle();
        if(p!=nullptr){
            p->Hold();
            m_particles.push_back(p);
        }
        return p;
    }else{
        return nullptr;
    }

}

void CBaseParticleEmitter::InitParticle(CParticle* p)
{
    if(p!=nullptr)
    {
        int randomness_x=0;
        int randomness_y=0;
        int p_x=m_pos_x+randomness_x;
        int p_y=m_pos_y+randomness_y;
        p->SetLife(m_particle_max_life);
        p->SetPosX(p_x); //is the screen coordinates
        p->SetPosY(p_y);
        p->SetSize(1);
        //according to type change particle velocity ignore acceleration
        p->SetVelocityX(0);
        p->SetVelocityY(0);
        p->SetAX(0);
        p->SetAY(0);
        p->SetTextureName("dot");
    }
}
void CBaseParticleEmitter::Update()
{
    for(size_t i=0; i<m_particles.size();)
    {
        CParticle* p=m_particles[i];
        if(p->IsDead())
        {

            if(m_emitter_life<0)
            {
                p->Release();
                m_particles.erase(m_particles.begin()+i);
            }
            else
            {
                InitParticle(p);
                i++;
            }
        }
        else
        {
            p->AutoUpdate();
            //Add Force to particles according to emmiter type;
            int life=p->GetLife();
            float lifespan=life*1.0/m_particle_max_life;
            if(lifespan>0.4 && lifespan<0.6){
                p->SetSize(3);
            }else if(lifespan>=0.6 && lifespan<0.8){
                p->SetSize(2);
            }else{
                p->SetSize(1);
            }
            if(p->GetVelocityX()==0 || p->GetVelocityY()==0){
                int rand_num_x=rand()%5-3;
                int rand_num_y=rand()%3-4;
                p->SetAX(rand_num_x);
                p->SetAY(rand_num_y);
            }else{
                int vx=p->GetVelocityX();
                int vy=p->GetVelocityY();
                int ax=(0-vx)/4;
                int ay=(0-vy)/4;
                p->SetAX(ax);
                p->SetAY(ay);

            }
            i++;
        }
    }
    if(m_particles.size()<m_max_particle_cnt){
        for(int i=0;i<m_particle_regenerate;i++){
            CParticle* p=CreateParticle();
            InitParticle(p);
        }
    }
    m_emitter_life--;
    //

    //GE_LOG("Pool Size===%d,%d\n",m_particles.size(),m_particle_pool->GetParicleCnt());
}

