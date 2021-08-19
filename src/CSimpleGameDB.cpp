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
    std::string obj_type=GetObjectType(object_id);
    int prop_id=GetPropId(obj_type,prop_name);
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

void CSimpleGameDB::SetTextData(std::string global_prop,std::string text)
{
    //TODO 判断insert还是update,update 的时候不应该push back
    if(m_globaltype_map.find(global_prop)!=m_globaltype_map.end())
    {
        if(m_globaltype_map[global_prop]==DataType::TEXT)
        {
            int pos=GetIntData(global_prop);
            //update element
            m_text_data[pos]=text;
        }
    }
    else
    {
        m_globaltype_map[global_prop]=DataType::TEXT;
        m_text_data.push_back(text);
        int pos=m_text_data.size()-1;
        SetIntData(global_prop,pos);
    }
}

std::string CSimpleGameDB::GetTextData(std::string global_prop)
{
    if(HasData(global_prop))
    {
        if(m_globaltype_map.find(global_prop)!=m_globaltype_map.end())
        {
            if(m_globaltype_map[global_prop]==DataType::TEXT)
            {
                int pos= GetIntData(global_prop);
                return m_text_data[pos];
            }
            else
            {
                int intval= GetIntData(global_prop);
                return std::to_string(intval);
            }
        }
        else
        {
            int intval= GetIntData(global_prop);
            return std::to_string(intval);
        }
    }
    else
    {
        return "";
    }


}

bool CSimpleGameDB::HasData(std::string global_prop)
{
    if(m_database.find(global_prop)!=m_database.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CSimpleGameDB::SetObjectData(int obj_id,int prop_id,int data)
{
    std::string key_name=GetKeyName(obj_id,prop_id);
    SetIntData(key_name,data);
}

std::string CSimpleGameDB::GetObjectText(int obj_id,std::string prop_name)
{
    std::string key_name=GetKeyName(obj_id,prop_name);
    int pos=GetIntData(key_name);
    if(pos>=0 && pos<(int)m_text_data.size())
    {
        std::string text=m_text_data[pos];
        return m_text_data[pos];
    }
    else
    {
        return "";
    }
}

void CSimpleGameDB::SetObjectText(int obj_id,std::string prop_name,
                                  std::string text)
{
    m_text_data.push_back(text);
    int pos=m_text_data.size()-1;
    std::string key_name=GetKeyName(obj_id,prop_name);
    SetIntData(key_name,pos);
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
std::string CSimpleGameDB::GetObjectType(int obj_id)
{
    return m_object_type[obj_id];
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

int CSimpleGameDB::StoreObject(std::string obj_name,
                               std::string obj_lable,std::string obj_type)
{
    //不支持多线程
    m_object_name.push_back(obj_name);
    m_object_label.push_back(obj_lable);
    m_object_type.push_back(obj_type);
    int obj_id=m_object_name.size()-1;
    m_object_id[obj_name]=obj_id;
    return obj_id;
}

void CSimpleGameDB::AddPropToType(std::string obj_type,std::string prop_name,
                                  std::string prop_label,DataType type)
{
    //不支持多线程，一次性载入
    int position;
    if(m_prop_name.find(obj_type)!=m_prop_name.end())
    {
        m_prop_name[obj_type].push_back(prop_name); //map []操作符返回的是引用
        m_prop_label[obj_type].push_back(prop_label); //map[]返回引用可以直接修改
        m_prop_datatype[obj_type].push_back(type);
    }
    else
    {
        std::vector<std::string> namelist;
        std::vector<std::string> labellist;
        std::vector<DataType> typelist;
        namelist.push_back(prop_name);
        labellist.push_back(prop_label);
        typelist.push_back(type);
        m_prop_name[obj_type]=namelist;
        m_prop_label[obj_type]=labellist;
        m_prop_datatype[obj_type]=typelist;
    }
    position=m_prop_name[obj_type].size()-1;
    std::string key_name=GetPropIntKey(obj_type,prop_name);
    m_prop_id[key_name]=position;
}

CSimpleGameDB::DataType CSimpleGameDB::GetPropType(std::string obj_type
        ,int prop_id)
{

    if(m_prop_datatype.find(obj_type)!=m_prop_datatype.end())
    {

        return m_prop_datatype[obj_type][prop_id];
    }
    else
    {
        return CSimpleGameDB::DataType::UNKNOWN;
    }
}

CGameDatabase::DataType CSimpleGameDB::GetPropType(std::string obj_type
        ,std::string prop_name)
{
    if(m_prop_name.find(obj_type)!=m_prop_name.end())
    {
        std::vector<std::string> names=m_prop_name[obj_type];
        for(size_t i=0; i<names.size(); i++)
        {
            if(prop_name.compare(names[i])==0)
            {
                return GetPropType(obj_type,i);
            }
        }
        return CSimpleGameDB::DataType::UNKNOWN;

    }
    else
    {
        return CSimpleGameDB::DataType::UNKNOWN;
    }
}

void CSimpleGameDB::CreateList(std::string list_name)
{
    if(m_object_list.find(list_name)!=m_object_list.end())
    {
        return;
    }
    else
    {
        std::vector<int> objlist;
        m_object_list[list_name]=objlist;
    }
}

void CSimpleGameDB::AddObjectToList(std::string list_name,int obj_id)
{
    if(m_object_list.find(list_name)!=m_object_list.end())
    {
        m_object_list[list_name].push_back(obj_id);
    }
    else
    {
        std::vector<int> objlist;
        m_object_list[list_name]=objlist;
        m_object_list[list_name].push_back(obj_id);
    }
}


void CSimpleGameDB::AddObjectToList(std::string list_name,std::string object_name)
{
    int oid= GetObjectId(object_name);
    AddObjectToList(list_name,oid);
}


void CSimpleGameDB::RemoveObjectFromList(std::string list_name,int obj_id)
{
    if(m_object_list.find(list_name)!=m_object_list.end())
    {
        std::vector<int>& olist=m_object_list[list_name];
        std::vector<int>::iterator it=find(olist.begin()
                                           ,olist.end(),obj_id);
        if(it!=olist.end())
        {
            olist.erase(it);
        }
    }

}

std::vector<int> CSimpleGameDB::GetListObjectIds(std::string list_name)
{
    if(m_object_list.find(list_name)!=m_object_list.end())
    {
        return m_object_list[list_name];
    }
    else
    {
        std::vector<int> tmplist;
        return tmplist;
    }
}

void CSimpleGameDB::CreateInventory(std::string list_name)
{
    CreateList(list_name);
    if(m_inventory.find(list_name)!=m_inventory.end())
    {
        return;
    }
    else
    {
        std::map<int,int> inv;
        m_inventory[list_name]=inv;
    }
}

void CSimpleGameDB::AddObjectToInventory(std::string list_name,int obj_id)
{
    if(m_inventory.find(list_name)!=m_inventory.end())
    {
        std::map<int,int>& inv=m_inventory[list_name];
        if(inv.find(obj_id)!=inv.end())
        {
            inv[obj_id]++;
        }
        else
        {
            AddObjectToList(list_name,obj_id);
            inv[obj_id]=1;
        }
    }
    else
    {
        CreateInventory(list_name);
        AddObjectToInventory(list_name,obj_id);
    }
}

void CSimpleGameDB::RemoveObjectFromInventory(std::string list_name,int obj_id)
{
    if(m_inventory.find(list_name)!=m_inventory.end())
    {
        std::map<int,int>& inv=m_inventory[list_name];
        if(inv.find(obj_id)!=inv.end())
        {
            if(inv[obj_id]==1)
            {
                inv.erase(inv.find(obj_id));
                RemoveObjectFromList(list_name,obj_id);
            }
            else
            {
                inv[obj_id]--;
            }
        }

    }
}

std::string CSimpleGameDB::GetListKeyName(int parent_id,std::string list_name)
{
    if(parent_id<0)
    {
        return list_name;
    }
    else
    {
        return list_name+"_"+std::to_string(parent_id);
    }
}

void CSimpleGameDB::CreateObjectList(int parent_id,std::string list_name)
{
    std::string list_name_new=GetListKeyName(parent_id,list_name);
    CreateList(list_name_new);
}

void CSimpleGameDB::CreateObjectInventory(int parent_id,std::string list_name)
{
    std::string list_name_new=GetListKeyName(parent_id,list_name);
    CreateInventory(list_name_new);
}

void CSimpleGameDB::AddObjectToObjectList(int parent_id,
        std::string list_name,int obj_id)
{
    std::string list_name_new=GetListKeyName(parent_id,list_name);
    AddObjectToList(list_name_new,obj_id);
}

void CSimpleGameDB::AddObjectToObjectInventory(int parent_id,
        std::string list_name,int obj_id)
{
    std::string list_name_new=GetListKeyName(parent_id,list_name);
    AddObjectToInventory(list_name_new,obj_id);
}

void CSimpleGameDB::RemoveObjectFromObjectList(int parent_id,
        std::string list_name
        ,int obj_id)
{
    std::string list_name_new=GetListKeyName(parent_id,list_name);
    RemoveObjectFromList(list_name_new,obj_id);
}

void CSimpleGameDB::RemoveObjectFromObjectInventory(int parent_id,
        std::string list_name,int obj_id)
{
    std::string list_name_new=GetListKeyName(parent_id,list_name);
    RemoveObjectFromInventory(list_name_new,obj_id);
}

std::vector<int> CSimpleGameDB::GetObjectListIds(int parent_id
        ,std::string list_name)
{
    std::string list_name_new=GetListKeyName(parent_id,list_name);
    return GetListObjectIds(list_name_new);
}
