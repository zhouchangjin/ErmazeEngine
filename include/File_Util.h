#ifndef FILE_UTIL_H_INCLUDED
#define FILE_UTIL_H_INCLUDED

//不引用下面两行不会编译出错，但是必须引用。
#include <string>
#include <vector>
#include <map>

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
    };

    template<class Type,class DataType>
    struct chunk_loader{
        chunk* chunk_def;
        typedef void (Type::*Setter)(DataType);
        Setter chunk_setter;
    };

}


#endif // FILE_UTIL_H_INCLUDED
