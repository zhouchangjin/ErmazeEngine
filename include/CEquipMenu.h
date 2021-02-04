#ifndef CEQUIPMENU_H
#define CEQUIPMENU_H

#include <common_struct.h>
#include <IMenuProcess.h>

class CEquipMenu:public IMenuProcess
{
    public:
        CEquipMenu();
        virtual ~CEquipMenu();
        void Choose(int obj_id,int menu_id=0);
        void Hover(int obj_id,int menu_id=0);
        void Expand(int obj_id,int menu_id=0);

    protected:

    private:
};

#endif // CEQUIPMENU_H
