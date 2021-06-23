#include "CParticleSystem.h"

CParticleSystem::CParticleSystem()
{
    //ctor
}

CParticleSystem::~CParticleSystem()
{
    //dtor
    for(size_t i=0;i<m_particle_emitters.size();i++){
        CBaseParticleEmitter* emmiter=m_particle_emitters[i];
        delete emmiter;
        emmiter=nullptr;
    }
    m_particle_emitters.clear();
}


void CParticleSystem::Init(){
    //Load Particle Spritesheet
    m_image_db=CServiceLocator::GetService<CImageDB>
              (CServiceLocator::ServiceID::TEXTURE_DB);
}

void CParticleSystem::Update(){

    for(size_t i=0;i<m_particle_emitters.size();i++){
        CBaseParticleEmitter* emmiter=m_particle_emitters[i];
        emmiter->Update();
    }
}

void CParticleSystem::Draw(){
    for(size_t i=0;i<m_particle_emitters.size();i++){
        CBaseParticleEmitter* emmitter=m_particle_emitters[i];
        std::vector<CParticle*> particles=emmitter->GetParticles();
        CTiledTexture tiledtexture=m_image_db->GetTiledTextureBySheetName(m_particle_sheet_name);
        sdlutil2::RenderParticles(m_context,particles,tiledtexture);
    }
}

void CParticleSystem::AddEmmiter(CBaseParticleEmitter* particle_emmiter){
    m_particle_emitters.push_back(particle_emmiter);
}


