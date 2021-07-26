#ifndef CGAMEDATACHUNKFACTORY_H
#define CGAMEDATACHUNKFACTORY_H

#include <File_Util.h>
#include <iostream>

class CGameDataChunkFactory : public ge_fileutil::IChunkFactory
{
    public:
        /** Default constructor */
        CGameDataChunkFactory();
        /** Default destructor */
        virtual ~CGameDataChunkFactory();

        int InitializeChunk(std::string chunk_name);
        void* GetInstance();
        void AddObjectToListProp();
        void SetIntProperty(std::string prop_name,int intValue);
        void SetStrProperty(std::string prop_name,std::string chunk_value);
        void SetDoubleProperty(std::string prop_name,double chunk_value);
        void SetFloatProperty(std::string prop_name,float chunk_value);
        void AddIntToArray(std::string prop_name,int intValue);
        void AddDoubleToArray(std::string prop_name,double dblValue);
        void AddFloatToArray(std::string prop_name,float fltValue);
        uint32_t CalListSize(std::string ref_chunk);
        std::string GetRefObjVal(std::string chunk_name,std::string ref_chunk);
        void PrintChunk(std::string chunk_name);
        void PrintAll();
        int GetIntValue(std::string chunk_name,int seq=0);
        std::string GetStrValue(std::string chunk_name,int seq=0);
        double GetDblValue(std::string chunk_name,int seq=0);
        float GetFloatValue(std::string chunk_name,int seq=0);
        std::vector<std::string> GetStrList(std::string chunk_name);
        std::vector<int>  GetIntList(std::string chunk_name);
        std::vector<double> GetDblList(std::string chunk_name);
        std::vector<float> GetFloatList(std::string chunk_name);
        uint32_t GetElementCnt(std::string chunk_name);

    protected:
        int m_chunk_id=0;
        std::vector<std::string> m_chunk_name;
        std::map<int,int> m_chunk_size;
        std::map<std::string,std::vector<int>> m_chunk_map; //get chunk cnt;
        std::map<int,int> m_int_value;
        std::map<int,float> m_float_value;
        std::map<int,double> m_double_value;
        std::map<int,std::string> m_str_value;
        void Print(int cid);
    private:
};

#endif // CGAMEDATACHUNKFACTORY_H
