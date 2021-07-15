#include "CParticleSystem.h"

CParticleSystem::CParticleSystem()
{
    //ctor
}

CParticleSystem::~CParticleSystem()
{
    //dtor
    for(size_t i=0;i<m_particle_emitters.size();i++){
        IEmitter* emmiter=m_particle_emitters[i];
        delete emmiter;
        emmiter=nullptr;
    }
    m_particle_emitters.clear();
}


void CParticleSystem::Init(){
    //Load Particle Spritesheet
    m_image_db=CServiceLocator::GetService<CImageDB>
              (CServiceLocator::ServiceID::TEXTURE_DB);
    for(size_t i=0;i<m_particle_emitters.size();i++){
        IEmitter* emmiter=m_particle_emitters[i];
        emmiter->Init();
    }
}

void CParticleSystem::Update(){

    for(size_t i=0;i<m_particle_emitters.size();i++){
        IEmitter* emmiter=m_particle_emitters[i];
        emmiter->Update();
        if(emmiter->IsDead()){
            GE_LOG("emmiter dead\n");
            m_particle_emitters.erase(m_particle_emitters.begin()+i);
            delete emmiter;
            emmiter=nullptr;
            i--;
        }
    }
}

void CParticleSystem::Draw(){
    for(size_t i=0;i<m_particle_emitters.size();i++){
        IEmitter* emmitter=m_particle_emitters[i];
        std::vector<CParticle*> particles=emmitter->GetParticles();
        CTiledTexture tiledtexture=m_image_db->GetTiledTextureBySheetName(m_particle_sheet_name);
        sdlutil2::RenderParticles(m_context,particles,tiledtexture);
    }
}

void CParticleSystem::AddEmitter(IEmitter* particle_emmiter){
    m_particle_emitters.push_back(particle_emmiter);
}


