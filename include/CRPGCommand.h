#ifndef CRPGCOMMAND_H
#define CRPGCOMMAND_H

#include <string>


class CRPGCommand
{
    public:
        CRPGCommand();
        CRPGCommand(const CRPGCommand& copyentity);
        virtual ~CRPGCommand();
        enum target_type{
            Enemy,
            EnemyAll,
            Ally,
            AllyAll,
            EnemyGroup,
            Neutral,
            Space
        };

        int GetCommandId()const{return m_command_id;};
        std::string GetCommandName()const{return m_command_name;};
        target_type GetTargetType()const{return m_targe_type;};
        int GetTargetId()const{return m_target_id;};
        int GetCenterX()const{return m_center_x;};
        int GetCenterY()const{return m_center_y;};
        int GetObjectId()const{return m_object_id;};

        void SetCommandId(int command_id){m_command_id=command_id;};
        void SetCommandName(std::string cmd_name){m_command_name=cmd_name;};
        void SetTargetType(target_type type){m_targe_type=type;};
        void SetTargetId(int target_id){m_target_id=target_id;};
        void SetCenterX(int x){m_center_x=x;};
        void SetCenterY(int y){m_center_y=y;};
        void SetObjectId(int id){m_object_id=id;};


    protected:
        int m_object_id;
        int m_command_id;
        std::string m_command_name;
        target_type m_targe_type;
        int m_target_id;
        int m_center_x;
        int m_center_y;
    private:
};

#endif // CRPGCOMMAND_H
