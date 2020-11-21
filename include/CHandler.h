#ifndef CHANDLER_H
#define CHANDLER_H

#include <CGameEvent.h>

class CHandler
{
    public:
        CHandler();
        virtual ~CHandler();
        void Exec(CGameEvent* event);
        virtual void Call(CGameEvent* event)=0;
    protected:

    private:
};

#endif // CHANDLER_H
