#ifndef FILE_UTIL_H_INCLUDED
#define FILE_UTIL_H_INCLUDED

//不引用下面两行不会编译出错，但是必须引用。
#include <string>
#include <vector>

namespace ge_fileutil{
    enum chunk_type{
        CHUNK,
        VARCHUNK,
        STRING,
        INT,
        DOUBLE,
        FLOAT,
        BYTEARRAY,
        INTARRAY,
        DOUBLEARRAY,
        FLOATARRAY
    };

    struct chunk{
        std::string chunk_name;  //块名称
        chunk_type type; //
        uint32_t chunk_size; //固定大小
        bool madatory;       //必须数据块
        bool variable_size;  //可变大小
        bool is_list;//
        std::vector<chunk> children;
        chunk* parent_chunk;
        std::string ref_chunk_list;
        std::string ref_chunk;
    };

}


#endif // FILE_UTIL_H_INCLUDED
