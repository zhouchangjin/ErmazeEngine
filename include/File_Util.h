#ifndef FILE_UTIL_H_INCLUDED
#define FILE_UTIL_H_INCLUDED

//不引用下面两行不会编译出错，但是必须引用。
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <Str_Utilities.h>

namespace ge_fileutil{
    enum chunk_type{
        CHUNK,
        STRING,
        INT,
        DOUBLE,
        FLOAT,
        BYTE,
        CHAR,
        UNKNOWN_CHUNK
    };

    struct chunk{
        std::string chunk_name;  //块名称*
        chunk_type type;      //块类型*
        uint32_t chunk_size; //固定大小*
        bool mandatory;       //必须数据块*
        bool variable_size;  //可变大小*
        bool is_list;      //数据列表*
        std::vector<chunk> chunk_properties;   //*
        std::vector<chunk> chunk_list_entity; //normally only one element*
        chunk* parent_chunk;      //*
        std::string ref_chunk;  //*
        std::map<std::string,chunk_type> rule;
        std::map<std::string,int> chunk_size_rule;
    };

    template<class Type,class DataType>
    struct chunk_loader{
        typedef void (Type::*Setter)(DataType);
        Setter chunk_setter; //对象设值
    };

    class IChunkFactory{
        public:
            virtual int InitializeChunk(std::string chunk_name)=0;
            virtual void* GetInstance()=0;
            virtual void AddObjectToListProp()=0;
            virtual void SetIntProperty(std::string prop_name,int intValue)=0;
            virtual void SetStrProperty(std::string prop_name,std::string chunk_value)=0;
            virtual void SetDoubleProperty(std::string prop_name,double chunk_value)=0;
            virtual void SetFloatProperty(std::string prop_name,float chunk_value)=0;
            virtual void AddIntToArray(std::string prop_name,int intValue)=0;
            virtual void AddDoubleToArray(std::string prop_name,double dblValue)=0;
            virtual void AddFloatToArray(std::string prop_name,float fltValue)=0;
            virtual uint32_t CalListSize(std::string ref_chunk)=0;
            virtual std::string GetRefObjVal(std::string chunk_name,std::string ref_chunk)=0;
        protected:
        void* m_instance;
        private:
    };

    class CBinaryFileReader{
        public:
        CBinaryFileReader(std::string filename){m_file_name=filename;};
        ~CBinaryFileReader(){};
        void ReOpen();
        void Close();
        bool EndOfFile();
        void ReadInt(int& num);
        void ReadDouble(double& dbl);
        void ReadFloat(float& flt);
        void ReadStringWithFixSize(std::string& str,int strsize);
        protected:
            std::string m_file_name;
            std::ifstream m_istream;
        private:
    };

    void parse_chunk_file_bydef(CBinaryFileReader& infile,chunk& chunk_def,IChunkFactory* factory);

}


#endif // FILE_UTIL_H_INCLUDED
