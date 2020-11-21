#include "CHandler.h"

CHandler::CHandler()
{
    //ctor
}

CHandler::~CHandler()
{
    //dtor
}

void CHandler::Exec(CGameEvent* event){
    Call(event);
}
