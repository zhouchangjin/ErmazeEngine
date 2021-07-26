#ifndef CGAME_H
#define CGAME_H
#include "CGameEngine.h"
class CGame
{
    public:
        CGame();
        virtual ~CGame();
        void Run();
    protected:

    private:
        CGameEngine m_engine;
};

#endif // CGAME_H
