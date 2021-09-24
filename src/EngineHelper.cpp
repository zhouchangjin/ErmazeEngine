#include <EngineHelper.h>


namespace ge_helper{

    std::function<int()> GetDiceRoller(int minv,int maxv){
        CRandomGenerator* gen=CServiceLocator::
               GetService<CRandomGenerator>
               (CServiceLocator::ServiceID::RANDOM_ENGINE);
        return gen->GetDice(minv,maxv);
    }


}
