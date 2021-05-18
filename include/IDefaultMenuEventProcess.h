#ifndef IDEFAULTMENUEVENTPROCESS_H
#define IDEFAULTMENUEVENTPROCESS_H

#include <CMenuInputEvent.h>

class IDefaultMenuEventProcess
{
    public:
        IDefaultMenuEventProcess();
        virtual ~IDefaultMenuEventProcess();
        virtual void ProcessInput(CMenuInputEvent event)=0;

    protected:

    private:
};

#endif // IDEFAULTMENUEVENTPROCESS_H
