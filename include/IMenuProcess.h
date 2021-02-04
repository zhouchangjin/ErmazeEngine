#ifndef IMENUPROCESS_H
#define IMENUPROCESS_H


class IMenuProcess
{
    public:
        IMenuProcess();
        virtual ~IMenuProcess();
        virtual void Choose(int obj_id,int menu_id=0)=0;
        virtual void Hover(int obj_id,int menu_id=0)=0;
        virtual void Expand(int obj_id,int menu_id=0)=0;
    protected:

    private:
};

#endif // IMENUPROCESS_H
