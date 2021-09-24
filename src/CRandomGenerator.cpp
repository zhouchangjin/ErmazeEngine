#include "CRandomGenerator.h"
std::random_device CRandomGenerator::random_dev;
std::default_random_engine CRandomGenerator::engine=
    std::default_random_engine(random_dev());
CRandomGenerator::CRandomGenerator()
{
    auto seed=std::chrono::system_clock::now().time_since_epoch().count();
    engine.seed(seed);
}

CRandomGenerator::~CRandomGenerator()
{
    //dtor
}


void CRandomGenerator::SetMinMax(int min_v,int max_v)
{

    m_min=min_v;
    m_max=max_v;

}

std::function<int()> CRandomGenerator::
GetDice(int min_value,int max_value)
{
    std::uniform_int_distribution<int> distribution(min_value,max_value);
    return std::bind(distribution,engine);
}
