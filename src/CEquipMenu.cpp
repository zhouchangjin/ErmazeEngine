#include "CEquipMenu.h"

CEquipMenu::CEquipMenu()
{
    //ctor
}

CEquipMenu::~CEquipMenu()
{
    //dtor
}

void CEquipMenu::Choose(int obj_id,int menu_id){
    GE_LOG("you have chosen menu %d, obj %d\n",menu_id,obj_id);
}

void CEquipMenu::Hover(int obj_id,int menu_id){

}

void CEquipMenu::Expand(int obj_id,int menu_id){

}
