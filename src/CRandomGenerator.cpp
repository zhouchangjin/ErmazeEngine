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
    //注意bind传参传引用否则参数进行软拷贝会生成重复的engine，进而出现重复的随机序列
    //https://stackoverflow.com/questions/23449243/c-how-to-correctly-bind-a-default-random-engine-to-two-different-uniform-int
    return std::bind(distribution,std::ref(engine));
}
