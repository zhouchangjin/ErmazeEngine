#ifndef ENGINEHELPER_H_INCLUDED
#define ENGINEHELPER_H_INCLUDED

#include <CServiceLocator.h>
#include <CRandomGenerator.h>

namespace ge_helper{

    typedef std::function<int()> GE_Dice;
    std::function<int()> GetDiceRoller(int minv,int maxv);


}


#endif // ENGINEHELPER_H_INCLUDED
