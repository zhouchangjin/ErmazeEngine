#include "CRPGCommand.h"

CRPGCommand::CRPGCommand()
{
    //ctor
}

CRPGCommand::~CRPGCommand()
{
    //dtor
}

CRPGCommand::CRPGCommand(const CRPGCommand& copyentity){

   m_center_x=copyentity.GetCenterX();
   m_center_y=copyentity.GetCenterY();
   m_command_id=copyentity.GetCommandId();
   m_command_name=copyentity.GetCommandName();
   m_targe_type=copyentity.GetTargetType();
   m_target_id=copyentity.GetTargetId();
   m_object_id=copyentity.GetObjectId();
}
