#ifndef CGAMEDATABASE_H
#define CGAMEDATABASE_H

#include <string>

class CGameDatabase
{
public:
    CGameDatabase();
    virtual ~CGameDatabase();
    virtual void SetIntData(std::string global_prop,int data)=0;
    virtual int GetIntData(std::string global_prop)=0;
    virtual void SetObjectData(int obj_id,
                               int prop_id,int data)=0;
    virtual void SetObjectData(int obj_id,std::string prop_name,int data)=0;
    virtual int GetObjectData(int obj_id,std::string prop_name)=0;
    virtual int GetObjectData(int obj_id,int prop_id)=0;
    virtual int GetPropId(std::string obj_type,std::string prop_name)=0;
    virtual int GetObjectId(std::string object_name)=0;
    virtual std::string GetObjectName(int object_id)=0;
    virtual std::string GetPropName(std::string obj_type,int prop_id)=0;
    virtual std::string GetObjectLabel(int object_id)=0;
    virtual std::string GetPropLabel(std::string obj_type,int prop_id)=0;
    virtual void StoreObject(std::string obj_name,std::string obj_label,
                             std::string obj_type)=0;
    virtual void AddPropToType(std::string obj_type,std::string prop_name,
                               std::string prop_lable)=0;
protected:

private:
};

#endif // CGAMEDATABASE_H