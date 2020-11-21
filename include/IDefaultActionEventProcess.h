#ifndef IDEFAULTACTIONEVENTPROCESS_H
#define IDEFAULTACTIONEVENTPROCESS_H
#include <CActionEvent.h>

class IDefaultActionEventProcess
{
    public:
        IDefaultActionEventProcess();
        virtual ~IDefaultActionEventProcess();
        virtual void OnAction(CActionEvent event)=0;

    protected:

    private:
};

#endif // IDEFAULTACTIONEVENTPROCESS_H
