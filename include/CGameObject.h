#ifndef CGAMEOBJECT_H
#define CGAMEOBJECT_H


class CGameObject
{
    public:
        CGameObject();
        virtual ~CGameObject();
        int GetX()const{return m_x;};
        int GetY()const{return m_y;};
        void SetX(int x){m_x=x;};
        void SetY(int y){m_y=y;};

    protected:

        int m_x=0;
        int m_y=0;

    private:
};

#endif // CGAMEOBJECT_H
