#ifndef CGAMECONTEXT_H
#define CGAMECONTEXT_H


#include "common_struct.h"

class CGameContext
{
    public:
        CGameContext();
        virtual ~CGameContext();
        virtual void Init(int window_width,int window_height)=0;
        virtual void ClearUp()=0;
        virtual ge_common_struct::input_event EventCatch()=0;
        virtual ge_common_struct::ge_rect GetWindowSize()=0;
        virtual unsigned int GetTicks()=0;
        virtual void DelayTime(unsigned int time)=0;
        virtual void* GetFont()const=0;
    protected:

    private:

};

#endif // CGAMECONTEXT_H
