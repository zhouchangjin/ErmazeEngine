#include "CProjectileEmitter.h"

CProjectileEmitter::CProjectileEmitter()
{
    //ctor
}

CProjectileEmitter::~CProjectileEmitter()
{
    //dtor
}

void CProjectileEmitter::Init(){

}

void CProjectileEmitter::Update(){
    m_projectile.AutoUpdate();
}

void CProjectileEmitter::SetProjectile(CProjectile projectile){
    m_projectile.SetPosX(projectile.GetX());
    m_projectile.SetPosY(projectile.GetY());
    m_projectile.SetAX(projectile.GetAx());
    m_projectile.SetAY(projectile.GetAy());
    m_projectile.SetLife(projectile.GetLife());
    m_projectile.SetVelocityX(projectile.GetVelocityX());
    m_projectile.SetVelocityY(projectile.GetVelocityY());
    m_projectile.SetTextureName(projectile.GetTextureName());
}

std::vector<CParticle*> CProjectileEmitter::GetParticles(){

    std::vector<CParticle*> partiles;
    partiles.push_back(&m_projectile);
    return partiles;

}
