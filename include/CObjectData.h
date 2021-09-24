#ifndef COBJECTDATA_H
#define COBJECTDATA_H


class CObjectData
{
    public:
        /** Default constructor */
        CObjectData();
        /** Default destructor */
        virtual ~CObjectData();

        void SetHp(int hp){m_hp=hp;};
        void SetHpmax(int hpmax){m_hpmax=hpmax;};
        void SetObjectId(int object_id){m_object_id=object_id;};
        void SetAlive(bool alive){m_alive=alive;};

        void SetStrength(int str){m_strength=str;};
        void SetConstitution(int con){m_constitution=con;};
        void SetInteligence(int inteli){m_inteligence=inteli;};
        void SetDexterity(int dex){m_dexterity=dex;};
        void SetSpeed(int spd){m_speed=spd;};

        int GetHp()const{return m_hp;};
        int GetHpmax()const{return m_hpmax;};
        bool IsAlive()const{return m_alive;};
        int GetObjectId()const{return m_object_id;};

        int GetStrength()const{return m_strength;};
        int GetConstitution()const{return m_constitution;};
        int GetInteligence()const{return m_inteligence;};
        int GetDexterity()const{return m_dexterity;};
        int GetSpeed()const{return m_speed;};

    protected:

        int m_object_id;
        int m_hp;
        int m_hpmax;

        int m_strength=1;
        int m_constitution=1;
        int m_inteligence=1;
        int m_dexterity=1;
        int m_speed=1;

        bool m_alive;

    private:
};

#endif // COBJECTDATA_H
