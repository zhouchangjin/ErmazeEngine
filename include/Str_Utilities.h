#ifndef STR_UTILITIES_H_INCLUDED
#define STR_UTILITIES_H_INCLUDED

#include <string>
#include <vector>
#include <regex>

namespace ge_str_utilities{

    //JAVA中Split方法
    std::vector<std::string> SplitStr(const std::string& str,char delim = ' ');

    std::vector<float> SplitStrToFloatArray(const std::string& str,char delim= ' ');

    std::vector<int> SplitStrToIntArray(const std::string& str,char delim=' ');

    //JAVA中Trim
    std::string TrimStr(std::string input);

    //Utf8字符长度
    int Utf8Strlen(const std::string& str);

    /*
     *逐字显示方法，模拟对话框中文字逐渐显示（打字机效果）
     *对话框中一行为line，显示第pos位置的字符串，返回所有行。pos以后的不显示
     */
    std::vector<std::string> SplitByUTF8CharPos(const std::string& input,
                                            int line,int pos);
    //JAVA中ReplaceAll
    void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

    //包含子串
    bool Contains(const std::string& original,std::string sub);

    std::vector<std::string> SearchPattern(const std::string& str,const std::regex reg);

}


#endif // STR_UTILITIES_H_INCLUDED
