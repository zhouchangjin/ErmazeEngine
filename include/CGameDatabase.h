#ifndef CGAMEDATABASE_H
#define CGAMEDATABASE_H

#include <vector>
#include <map>
#include <string>

class CGameDatabase
{
public:
    enum DataType{
        UNKNOWN,
        INTEGER,
        TEXT,
        OBJECT_ID,
        TEXTURE_ID,
        SPRITE_ID,
        ICON_ID
    };
    CGameDatabase();
    virtual ~CGameDatabase();
    virtual void SetIntData(std::string global_prop,int data)=0;
    virtual int GetIntData(std::string global_prop)=0;
    virtual std::string GetTextData(std::string global_prop)=0;
    virtual void SetTextData(std::string global_prop,std::string text)=0;

    virtual bool HasData(std::string global_prop)=0;

    virtual void SetObjectData(int obj_id,
                               int prop_id,int data)=0;
    virtual void SetObjectData(int obj_id,std::string prop_name,int data)=0;
    virtual void SetObjectText(int obj_id,std::string prop_name,
                               std::string text)=0;
    virtual std::string GetObjectText(int obj_id,std::string prop_name)=0;
    virtual int GetObjectData(int obj_id,std::string prop_name)=0;
    virtual int GetObjectData(int obj_id,int prop_id)=0;
    virtual std::string GetObjectType(int obj_id)=0;
    virtual int GetPropId(std::string obj_type,std::string prop_name)=0;
    virtual int GetObjectId(std::string object_name)=0;
    virtual std::string GetObjectName(int object_id)=0;
    virtual std::string GetPropName(std::string obj_type,int prop_id)=0;
    virtual DataType GetPropType(std::string obj_type,std::string prop_name)=0;
    virtual DataType GetPropType(std::string obj_type,int prop_id)=0;
    virtual std::string GetObjectLabel(int object_id)=0;
    virtual std::string GetPropLabel(std::string obj_type,int prop_id)=0;
    virtual int StoreObject(std::string obj_name,std::string obj_label,
                             std::string obj_type)=0;
    virtual void AddPropToType(std::string obj_type,std::string prop_name,
                               std::string prop_label,
                               DataType type=DataType::INTEGER)=0;

    virtual void CreateList(std::string list_name)=0;
    virtual void AddObjectToList(std::string list_name,int obj_id)=0;
    virtual void AddObjectToList(std::string list_name,std::string object_name)=0;
    virtual void RemoveObjectFromList(std::string list_name,int obj_id)=0;

    virtual void CreateInventory(std::string list_name)=0;
    virtual void AddObjectToInventory(std::string list_name,int obj_type_id)=0;
    virtual void RemoveObjectFromInventory(std::string list_name,int obj_type_id)=0;

    virtual void CreateObjectList(int parent_id,std::string list_name)=0;
    virtual void AddObjectToObjectList(int parent_id,std::string list_name,int obj_id)=0;
    virtual void RemoveObjectFromObjectList(int parent_id,std::string list_name,int obj_id)=0;

    virtual void CreateObjectInventory(int parent_id,std::string list_name)=0;
    virtual void AddObjectToObjectInventory(int parent_id,std::string list_name,int obj_id)=0;
    virtual void RemoveObjectFromObjectInventory(int parent_id,std::string list_name,int obj_id)=0;

    virtual std::vector<int> GetListObjectIds(std::string list_name)=0;
    virtual std::vector<int> GetObjectListIds(int parent_id,std::string list_name)=0;

protected:

private:
};

#endif // CGAMEDATABASE_H
