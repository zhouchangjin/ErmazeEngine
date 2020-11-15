#ifndef STR_UTILITIES_H_INCLUDED
#define STR_UTILITIES_H_INCLUDED

#include <string>
#include <vector>

namespace ge_str_utilities{

    std::vector<std::string> Splitstr(const std::string& str,char delim = ' ');

    std::vector<float> SplitStrToFloatArray(const std::string& str,char delim= ' ');

    std::vector<int> SplitStrToIntArray(const std::string& str,char delim=' ');

    std::string TrimStr(std::string input);

    int utf8_strlen(const std::string& str);

    std::vector<std::string> SplitByUTF8CharPos(const std::string& input,
                                            int line,int pos);

}


#endif // STR_UTILITIES_H_INCLUDED
