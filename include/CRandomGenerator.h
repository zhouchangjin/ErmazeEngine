#ifndef CRANDOMGENERATOR_H
#define CRANDOMGENERATOR_H

#include <chrono>
#include <random>
#include <functional>

class CRandomGenerator
{
    public:
        /** Default constructor */
        CRandomGenerator();
        /** Default destructor */
        virtual ~CRandomGenerator();

        void SetMinMax(int min_value,int max_value);

        std::function<int()> GetDice(int min_value,int max_value);

    protected:
        static std::random_device random_dev;
        static std::default_random_engine engine;

        int m_min;
        int m_max;

    private:
};

#endif // CRANDOMGENERATOR_H
