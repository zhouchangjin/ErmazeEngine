#ifndef CSIMPLEGAMEDB_H
#define CSIMPLEGAMEDB_H

#include <CGameDatabase.h>
#include<algorithm>

class CSimpleGameDB : public CGameDatabase
{
public:
    CSimpleGameDB();
    virtual ~CSimpleGameDB();
    void SetIntData(std::string global_prop,int data);
    int GetIntData(std::string global_prop);
    std::string GetTextData(std::string global_prop);
    void SetTextData(std::string global_prop,std::string text);
    bool HasData(std::string global_prop);

    void SetObjectData(int obj_id,int prop_id,int data);
    void SetObjectData(int obj_id,std::string prop_name,int data);
    void SetObjectText(int obj_id,std::string prop_name,std::string text);
    int GetObjectData(int obj_id,std::string prop_name);
    int GetObjectData(int obj_id,int prop_id);
    std::string GetObjectType(int obj_id);
    std::string GetObjectText(int obj_id,std::string prop_name);
    int GetPropId(std::string obj_type,std::string prop_name);
    int GetObjectId(std::string object_name);
    std::string GetObjectName(int object_id);
    std::string GetPropName(std::string obj_type,int prop_id);
    DataType GetPropType(std::string obj_type,std::string prop_name);
    DataType GetPropType(std::string obj_type,int prop_id);
    std::string GetObjectLabel(int object_id);
    std::string GetPropLabel(std::string obj_type,int prop_id);
    int StoreObject(std::string obj_name,std::string obj_label,
                     std::string obj_type);
    void AddPropToType(std::string obj_type,std::string prop_name,
                       std::string prop_lable,DataType type=DataType::INTEGER);
    void CreateList(std::string list_name);
    void AddObjectToList(std::string list_name,int obj_id);
    void AddObjectToList(std::string list_name,std::string object_name);
    void RemoveObjectFromList(std::string list_name,int obj_id);

    std::vector<int> GetListObjectIds(std::string list_name);

    void CreateInventory(std::string list_name);
    void AddObjectToInventory(std::string list_name,int obj_id);
    void RemoveObjectFromInventory(std::string list_name,int obj_id);

    void CreateObjectList(int parent_id,std::string list_name);
    void AddObjectToObjectList(int parent_id,std::string list_name,int obj_id);
    void RemoveObjectFromObjectList(int parent_id,std::string list_name,int obj_id);

    void CreateObjectInventory(int parent_id,std::string list_name);
    void AddObjectToObjectInventory(int parent_id,std::string list_name,int obj_id);
    void RemoveObjectFromObjectInventory(int parent_id,std::string list_name,int obj_id);

    std::vector<int> GetObjectListIds(int parent_id,std::string list_name);





protected:

private:

    std::string GetKeyName(int object_id,int prop_id);
    std::string GetKeyName(int object_id,std::string prop_name);
    std::string GetKeyName(std::string object_name,std::string prop_name);
    std::string GetListKeyName(int parent_id,std::string list_name);

    std::string GetPropIntKey(std::string obj_type,std::string prop_name);

    std::map<std::string,int> m_database;
    //global_type
    std::map<std::string,DataType> m_globaltype_map;
    //object
    std::map<std::string,int> m_object_id;
    std::vector<std::string> m_object_type;
    std::vector<std::string> m_object_name;
    std::vector<std::string> m_object_label;
    std::vector<std::string> m_text_data;
    //list
    std::map<std::string,std::vector<int>> m_object_list;
    std::map<std::string,std::map<int,int>> m_inventory;
    //metadata below
    std::map<std::string,int> m_prop_id;
    std::map<std::string,std::vector<std::string>> m_prop_name;
    std::map<std::string,std::vector<std::string>> m_prop_label;
    std::map<std::string,std::vector<DataType>> m_prop_datatype;

};

#endif // CSIMPLEGAMEDB_H
