#ifndef IDEFAULTEVENTPROCESS_H
#define IDEFAULTEVENTPROCESS_H

#include <CInputEvent.h>

class IDefaultEventProcess
{
    public:
        IDefaultEventProcess();
        virtual ~IDefaultEventProcess();
        virtual void ProcessInput(CInputEvent event)=0;

    protected:

    private:
};

#endif // IDEFAULTEVENTPROCESS_H
