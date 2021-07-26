#include "CDatabaseLoader.h"

CDatabaseLoader::CDatabaseLoader()
{
    //ctor
}

CDatabaseLoader::~CDatabaseLoader()
{
    //dtor
}

void CDatabaseLoader::LoadObjects(CGameDataChunkFactory* factory)
{
    std::string object_type_name=factory->GetStrValue("obj_type");
    std::vector<std::string> propList=factory->GetStrList("prop_name");
    std::vector<std::string> propLabel=factory->GetStrList("prop_label");
    std::vector<int> propType=factory->GetIntList("prop_type");

    size_t name_cnt=propList.size();
    size_t lable_cnt=propLabel.size();
    size_t type_cnt=propType.size();
    int name_prop_seq=-1;
    int label_prop_seq=-1;
    if(name_cnt==lable_cnt && lable_cnt==type_cnt)
    {
        for(size_t i=0; i<propList.size(); i++)
        {
            std::string prop_name=propList[i];
            std::string prop_label=propLabel[i];
            int prop_type=propType[i];
            CGameDatabase::DataType type;
            if(prop_type==1){
                type=CGameDatabase::DataType::INTEGER;
            }else if(prop_type==2){
                type=CGameDatabase::DataType::TEXT;
            }

            if(prop_name.compare("sprite")==0){
                type=CGameDatabase::DataType::SPRITE_ID;
            }else if(prop_name.compare("icon")==0){
                type=CGameDatabase::DataType::ICON_ID;
            }else if(prop_name.compare("name")==0){
                name_prop_seq=i;
            }else if(prop_name.compare("label")==0){
                label_prop_seq=i;
            }
            m_gamedatabase->AddPropToType(object_type_name,prop_name,prop_label,type);
        }
    }

    uint32_t cnt=factory->GetElementCnt("data_ele");
    for(uint32_t i=0;i<cnt;i++){
        std::string object_name=object_type_name+std::to_string(i);
        if(name_prop_seq>=0){
            int name_seq=i*name_cnt+name_prop_seq;
            object_name=factory->GetStrValue("data_prop",name_seq);
        }
        std::string object_label=object_name;
        if(label_prop_seq>=0){
            int label_seq=i*name_cnt+label_prop_seq;
            object_label=factory->GetStrValue("data_prop",label_seq);
        }
        int oid=m_gamedatabase->StoreObject(object_name,object_label,
                                            object_type_name);

        for(size_t j=0;j<name_cnt;j++){
            int seq=i*name_cnt+j;
            if(propType[j]==1){
                 int data=factory->GetIntValue("data_prop",seq);
                 m_gamedatabase->SetObjectData(oid,propList[j],data);
            }else if(propType[j]==2){
                std::string data=factory->GetStrValue("data_prop",seq);
                m_gamedatabase->SetObjectText(oid,propList[j],data);
            }
        }
    }
}
