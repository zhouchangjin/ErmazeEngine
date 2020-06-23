#include "CGame.h"

CGame::CGame()
{
    //ctor
}

CGame::~CGame()
{
    //dtor
}

void CGame::Run(){
    m_engine.Init();

    while(m_engine.Running()){
        m_engine.HandleEvent();
        m_engine.Update();
        m_engine.Draw();
    }


}
