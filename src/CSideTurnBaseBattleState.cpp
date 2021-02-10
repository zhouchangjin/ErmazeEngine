#include "CSideTurnBaseBattleState.h"

CSideTurnBaseBattleState::CSideTurnBaseBattleState()
{
    //ctor
}

CSideTurnBaseBattleState::~CSideTurnBaseBattleState()
{
    //dtor
}

void CSideTurnBaseBattleState::Init()
{
    //0代表状态不改变
    m_state_value=0;
    LoadUIDef();

}

void CSideTurnBaseBattleState::Cleanup()
{

}

void CSideTurnBaseBattleState::Pause()
{

}

void CSideTurnBaseBattleState::Resume()
{
   m_state_value=0;
}

void CSideTurnBaseBattleState::HandleEvent(ge_common_struct::input_event event)
{
}

void CSideTurnBaseBattleState::Update()
{

}

void CSideTurnBaseBattleState::Draw()
{

}

int CSideTurnBaseBattleState::GetStateValue()
{
   return this->m_state_value;
}

void CSideTurnBaseBattleState::PrepareData()
{

}

void CSideTurnBaseBattleState::LoadUIDef(){




}
