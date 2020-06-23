#include "CGameState.h"

CGameState::CGameState(){

}

CGameState::CGameState(CGameContext* context)
{
    this->m_context=context;
}

CGameState::~CGameState()
{
    //dtor
}
