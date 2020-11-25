#include "CSimpleGameDB.h"

CSimpleGameDB::CSimpleGameDB()
{
    //ctor
}

CSimpleGameDB::~CSimpleGameDB()
{
    //dtor
}

std::string CSimpleGameDB::GetKeyName(int object_id,int prop_id)
{

    return std::to_string(object_id)+"_"+std::to_string(prop_id);
}

std::string CSimpleGameDB::GetKeyName(int object_id,std::string prop_name)
{
    int prop_id=m_prop_id[prop_name];
    return GetKeyName(object_id,prop_id);
}

std::string CSimpleGameDB::GetKeyName(std::string object_name,
                                      std::string prop_name)
{
    int obj_id=m_object_id[object_name];
    return GetKeyName(obj_id,prop_name);
}

std::string CSimpleGameDB::GetPropIntKey(std::string obj_type,
        std::string prop_name)
{
    return obj_type+"_"+prop_name;
}

void CSimpleGameDB::SetIntData(std::string global_prop,int data)
{
    m_database[global_prop]=data;
}

int CSimpleGameDB::GetIntData(std::string global_prop)
{
    return m_database[global_prop];
}

void CSimpleGameDB::SetObjectData(int obj_id,int prop_id,int data)
{
    std::string key_name=GetKeyName(obj_id,prop_id);
    SetIntData(key_name,data);
}

void CSimpleGameDB::SetObjectData(int obj_id,std::string prop_name,int data)
{
    std::string key_name=GetKeyName(obj_id,prop_name);
    SetIntData(key_name,data);
}

int CSimpleGameDB::GetObjectData(int obj_id,int prop_id)
{
    std::string key_name=GetKeyName(obj_id,prop_id);
    return GetIntData(key_name);
}

int CSimpleGameDB::GetObjectData(int obj_id,std::string prop_name)
{
    std::string key_name=GetKeyName(obj_id,prop_name);
    return GetIntData(key_name);
}

int CSimpleGameDB::GetPropId(std::string obj_type,std::string prop_name)
{
    std::string key_name=GetPropIntKey(obj_type,prop_name);
    return m_prop_id[key_name];
}

int CSimpleGameDB::GetObjectId(std::string obj_name)
{
    return m_object_id[obj_name];
}

std::string CSimpleGameDB::GetObjectName(int obj_id)
{
    return m_object_name[obj_id];
}

std::string CSimpleGameDB::GetObjectLabel(int obj_id)
{
    return m_object_label[obj_id];
}

std::string CSimpleGameDB::GetPropName(std::string obj_type,int prop_id)
{
    if(m_prop_name.find(obj_type)!=m_prop_name.end())
    {
        std::vector<std::string> props=m_prop_name[obj_type];
        if(prop_id>=0 && prop_id<(int)props.size())
        {
            return props[prop_id];
        }
    }
    return "";
}

std::string CSimpleGameDB::GetPropLabel(std::string obj_type,int prop_id)
{
    if(m_prop_label.find(obj_type)!=m_prop_label.end())
    {
        std::vector<std::string> props=m_prop_label[obj_type];
        if(prop_id>=0 && prop_id<(int)props.size())
        {
            return props[prop_id];
        }
    }
    return "";
}

void CSimpleGameDB::StoreObject(std::string obj_name,
                                std::string obj_lable,std::string obj_type)
{
    m_object_name.push_back(obj_name);
    m_object_label.push_back(obj_lable);
    m_object_type.push_back(obj_type);
    m_object_id[obj_name]=m_object_name.size();
}

void CSimpleGameDB::AddPropToType(std::string obj_type,std::string prop_name,
                                  std::string prop_label)
{
    int position;
    if(m_prop_name.find(obj_type)!=m_prop_name.end()){
        m_prop_name[obj_type].push_back(prop_name); //map []操作符返回的是引用
        m_prop_label[obj_type].push_back(prop_label); //map[]返回引用可以直接修改
    }else{
        std::vector<std::string> namelist;
        std::vector<std::string> labellist;
        namelist.push_back(prop_name);
        labellist.push_back(prop_label);
        m_prop_name[obj_type]=namelist;
        m_prop_label[obj_type]=labellist;
    }
    position=m_prop_name[obj_type].size()-1;
    std::string key_name=GetPropIntKey(obj_type,prop_name);
    m_prop_id[key_name]=position;
}
