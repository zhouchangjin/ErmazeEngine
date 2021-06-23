#ifndef CPARTICLE_H
#define CPARTICLE_H
#include "common_struct.h"

class CParticle
{
    public:
        CParticle();
        virtual ~CParticle();
        CParticle(int x,int y,int vx,int vy,int life);
        int GetX()const{return m_pos_x;};
        int GetY()const{return m_pos_y;};
        int GetVelocityX()const{return m_velocity_x;};
        int GetVelocityY()const{return m_velocity_y;};
        int GetAx()const{return m_acc_x;};
        int GetAy()const{return m_acc_y;};
        int GetLife()const{return m_life;};
        std::string GetTextureName()const{return m_texture_name;};
        ge_common_struct::ge_adv_color GetColor()const{return m_color;};
        int GetSize()const{return m_size;};
        bool IsDead(){return m_life<=0;};
        bool IsIdle(){return m_is_idle;};
        bool IsTexture(){return m_particle_type==ParticleType::TEXTURE;};

        void Hold(){m_is_idle=false;};
        void Release(){m_is_idle=true;};
        void SetPosX(int x){m_pos_x=x;};
        void SetPosY(int y){m_pos_y=y;};
        void SetVelocityX(int vx){m_velocity_x=vx;};
        void SetVelocityY(int vy){m_velocity_y=vy;};
        void SetAX(int ax){m_acc_x=ax;};
        void SetAY(int ay){m_acc_y=ay;};
        void SetLife(int life){m_life=life;};
        void SetColor(ge_common_struct::ge_adv_color color){m_color=color;};
        void SetSize(int p_size){m_size=p_size;};
        void SetTextureName(std::string texturename){m_texture_name=texturename;};
        void AutoUpdate();
        enum ParticleType{
            DOT,
            LINE,
            SQUARE,
            TEXTURE
        };
    protected:
        int m_pos_x=0; //may actually offset;
        int m_pos_y=0;
        int m_velocity_x=0;
        int m_velocity_y=0;
        int m_acc_x=0;
        int m_acc_y=0;
        int m_life=-1;
        int m_size=1;
        bool m_is_idle=true;
        ge_common_struct::ge_adv_color m_color;
        ParticleType m_particle_type=ParticleType::DOT;
        std::string m_texture_name;

    private:
};

#endif // CPARTICLE_H
