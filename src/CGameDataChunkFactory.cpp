#include "CGameDataChunkFactory.h"

CGameDataChunkFactory::CGameDataChunkFactory()
{
    //ctor
}

CGameDataChunkFactory::~CGameDataChunkFactory()
{
    //dtor
}

void CGameDataChunkFactory::AddDoubleToArray(std::string prop_name,double value)
{
}

void CGameDataChunkFactory::AddFloatToArray(std::string prop_name,float value)
{

}

void CGameDataChunkFactory::AddIntToArray(std::string prop_name,int intv)
{

}

void CGameDataChunkFactory::AddObjectToListProp()
{


}


uint32_t CGameDataChunkFactory::CalListSize(std::string ref_chunk)
{
    if(m_chunk_map.find(ref_chunk)!=m_chunk_map.end())
    {
        std::vector<int> chunk_id= m_chunk_map[ref_chunk];
        if(chunk_id.size()==1)
        {
            int cid=chunk_id[0];
            if(m_int_value.find(cid)!=m_int_value.end())
            {
                int iValue=m_int_value[cid];
                //TODO depends different rule
                return iValue;
            }
            else
            {
                return 0;
            }

        }
        else
        {
            return chunk_id.size();
        }
    }
    return 0;
}


int CGameDataChunkFactory::InitializeChunk(std::string chunk_name)
{


    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        m_chunk_map[chunk_name].push_back(m_chunk_id);
    }
    else
    {
        std::vector<int> chunk_list;
        chunk_list.push_back(m_chunk_id);
        m_chunk_map[chunk_name]=chunk_list;
    }
    m_chunk_name.push_back(chunk_name);
    m_chunk_id++;
    return m_chunk_id;

}

void* CGameDataChunkFactory::GetInstance()
{
    return m_instance;
}

void CGameDataChunkFactory::SetDoubleProperty(std::string chunk_name,double val)
{
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        int cid=m_chunk_map[chunk_name].back();
        m_double_value[cid]=val;
    }

}

void CGameDataChunkFactory::SetFloatProperty(std::string chunk_name,float val)
{
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        int cid=m_chunk_map[chunk_name].back();
        m_float_value[cid]=val;
    }
}

void CGameDataChunkFactory::SetIntProperty(std::string chunk_name,int val)
{
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        int cid=m_chunk_map[chunk_name].back();
        m_int_value[cid]=val;
    }

}

void CGameDataChunkFactory::Print(int id)
{
    if(m_int_value.find(id)!=m_int_value.end())
    {
        std::cout<<id<<" "<< m_chunk_name[id]<<"="<<m_int_value[id]<<std::endl;
    }
    else if(m_str_value.find(id)!=m_str_value.end())
    {
        std::cout<<id<<" "<<  m_chunk_name[id] <<"="<<m_str_value[id]<<std::endl;
    }
    else
    {

        std::cout<<id<<" "<< m_chunk_name[id]<<" "<<std::endl;
    }

}

void CGameDataChunkFactory::PrintAll()
{
    for(int i=0; i<m_chunk_id; i++)
    {
        Print(i);
    }
}

void CGameDataChunkFactory::PrintChunk(std::string chunk_name)
{
    std::vector<int> ids=m_chunk_map[chunk_name];
    for(size_t i=0; i<ids.size(); i++)
    {
        int id=ids[i];
        Print(id);
    }
}

void CGameDataChunkFactory::SetStrProperty(std::string chunk_name,
        std::string val)
{
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        int cid=m_chunk_map[chunk_name].back();
        m_str_value[cid]=val;
    }


}

std::string CGameDataChunkFactory::GetRefObjVal(std::string chunk_name,
        std::string ref_chunk)
{
    if(m_chunk_map.find(ref_chunk)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[ref_chunk];
        size_t cnt=ids.size();
        if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
        {
            std::vector<int> cids= m_chunk_map[chunk_name];
            size_t ccnt=cids.size();
            int order=(ccnt-1)%cnt;
            int valueId=ids[order];
            if(m_int_value.find(valueId)!=m_int_value.end())
            {
                int val=m_int_value[valueId];
                return std::to_string(val);
            }
            else if(m_str_value.find(valueId)!=m_str_value.end())
            {
                std::string strval=m_str_value[valueId];
                return strval;
            }
        }

    }
    return "";
}

int CGameDataChunkFactory::GetIntValue(std::string chunk_name,int seq)
{


    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[chunk_name];
        if(ids.size()>0)
        {
            int id=ids[seq]; //only get first one
            if(m_int_value.find(id)!=m_int_value.end())
            {
                return m_int_value[id];
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {

        return 0;
    }

}

std::string CGameDataChunkFactory::GetStrValue(std::string chunk_name,int seq)
{

    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[chunk_name];
        if(ids.size()>0)
        {
            int id=ids[seq]; //only get first one
            if(m_str_value.find(id)!=m_str_value.end())
            {
                return m_str_value[id];
            }
            else
            {
                return "";
            }
        }
        else
        {
            return "";
        }
    }
    else
    {

        return "";
    }

}

double CGameDataChunkFactory::GetDblValue(std::string chunk_name,int seq)
{
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[chunk_name];
        if(ids.size()>0)
        {
            int id=ids[seq]; //only get first one
            if(m_double_value.find(id)!=m_double_value.end())
            {
                return m_double_value[id];
            }
            else
            {
                return 0.0;
            }
        }
        else
        {
            return 0.0;
        }
    }
    else
    {

        return 0.0;
    }


}

float CGameDataChunkFactory::GetFloatValue(std::string chunk_name,int seq)
{

    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[chunk_name];
        if(ids.size()>0)
        {
            int id=ids[seq]; //only get first one
            if(m_float_value.find(id)!=m_float_value.end())
            {
                return m_float_value[id];
            }
            else
            {
                return 0.0f;
            }
        }
        else
        {
            return 0.0f;
        }
    }
    else
    {

        return 0.0f;
    }


}

std::vector<std::string> CGameDataChunkFactory::GetStrList(
    std::string chunk_name)
{
    std::vector<std::string> strList;
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[chunk_name];
        for(size_t i=0; i<ids.size(); i++)
        {
            int id=ids[i];
            if(m_str_value.find(id)!=m_str_value.end())
            {

                strList.push_back(m_str_value[id]);
            }
        }
    }
    return strList;
}

std::vector<int> CGameDataChunkFactory::GetIntList(std::string chunk_name)
{
    std::vector<int> intList;
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[chunk_name];
        for(size_t i=0; i<ids.size(); i++)
        {
            int id=ids[i];
            if(m_int_value.find(id)!=m_int_value.end())
            {

                intList.push_back(m_int_value[id]);
            }
        }
    }
    return intList;
}

std::vector<double> CGameDataChunkFactory::GetDblList(std::string chunk_name)
{
    std::vector<double> dblList;
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[chunk_name];
        for(size_t i=0; i<ids.size(); i++)
        {
            int id=ids[i];
            if(m_double_value.find(id)!=m_double_value.end())
            {
                dblList.push_back(m_double_value[id]);
            }
        }
    }
    return dblList;
}

std::vector<float> CGameDataChunkFactory::GetFloatList(
    std::string chunk_name)
{
    std::vector<float> fltList;
    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end())
    {
        std::vector<int> ids=m_chunk_map[chunk_name];
        for(size_t i=0; i<ids.size(); i++)
        {
            int id=ids[i];
            if(m_float_value.find(id)!=m_float_value.end())
            {
                fltList.push_back(m_float_value[id]);
            }
        }
    }
    return fltList;
}

uint32_t CGameDataChunkFactory::GetElementCnt(std::string chunk_name){

    if(m_chunk_map.find(chunk_name)!=m_chunk_map.end()){
        return m_chunk_map[chunk_name].size();
    }else{
        return 0;
    }
}
