#ifndef CSIMPLEGAMEDB_H
#define CSIMPLEGAMEDB_H

#include <map>
#include <vector>
#include <CGameDatabase.h>


class CSimpleGameDB : public CGameDatabase
{
public:
    CSimpleGameDB();
    virtual ~CSimpleGameDB();
    void SetIntData(std::string global_prop,int data);
    int GetIntData(std::string global_prop);
    void SetObjectData(int obj_id,int prop_id,int data);
    void SetObjectData(int obj_id,std::string prop_name,int data);
    int GetObjectData(int obj_id,std::string prop_name);
    int GetObjectData(int obj_id,int prop_id);
    int GetPropId(std::string obj_type,std::string prop_name);
    int GetObjectId(std::string object_name);
    std::string GetObjectName(int object_id);
    std::string GetPropName(std::string obj_type,int prop_id);
    std::string GetObjectLabel(int object_id);
    std::string GetPropLabel(std::string obj_type,int prop_id);
    void StoreObject(std::string obj_name,std::string obj_label,
                     std::string obj_type);
    void AddPropToType(std::string obj_type,std::string prop_name,
                       std::string prop_lable);
protected:

private:

    std::string GetKeyName(int object_id,int prop_id);
    std::string GetKeyName(int object_id,std::string prop_name);
    std::string GetKeyName(std::string object_name,std::string prop_name);

    std::string GetPropIntKey(std::string obj_type,std::string prop_name);

    std::map<std::string,int> m_database;

    //object
    std::map<std::string,int> m_object_id;
    std::vector<std::string> m_object_type;
    std::vector<std::string> m_object_name;
    std::vector<std::string> m_object_label;


    //metadata below
    std::map<std::string,int> m_prop_id;
    std::map<std::string,std::vector<std::string>> m_prop_name;
    std::map<std::string,std::vector<std::string>> m_prop_label;

};

#endif // CSIMPLEGAMEDB_H
