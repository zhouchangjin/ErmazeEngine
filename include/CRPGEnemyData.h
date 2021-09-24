#ifndef CRPGENEMYDATA_H
#define CRPGENEMYDATA_H
#include <CObjectData.h>

class CRPGEnemyData:public CObjectData
{
    public:
        /** Default constructor */
        CRPGEnemyData();
        /** Default destructor */
        virtual ~CRPGEnemyData();

        int GetGold()const{return m_gold;};
        int GetExp()const{return m_exp;};
        void SetGold(int gold){m_gold=gold;};
        void SetExp(int exp){m_exp=exp;};

    protected:
        int m_gold;
        int m_exp;

    private:
};

#endif // CRPGENEMYDATA_H
