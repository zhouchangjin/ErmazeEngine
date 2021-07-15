#ifndef IEMITTER_H
#define IEMITTER_H

#include "common_struct.h"
#include <CParticle.h>

class IEmitter
{
    public:
        IEmitter();
        virtual ~IEmitter();
        virtual void Init()=0;
        virtual void Update()=0;
        virtual bool IsDead()=0;
        virtual int GetX()=0;
        virtual int GetY()=0;
        virtual void UpdateX(int x)=0;
        virtual void UpdateY(int y)=0;
        virtual std::vector<CParticle*> GetParticles()=0;
    protected:

    private:
};

#endif // IEMITTER_H
