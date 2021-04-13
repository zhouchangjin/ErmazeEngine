#ifndef STR_UTILITIES_H_INCLUDED
#define STR_UTILITIES_H_INCLUDED

#include <string>
#include <vector>
#include <regex>

namespace ge_str_utilities{

    //JAVA��Split����
    std::vector<std::string> SplitStr(const std::string& str,char delim = ' ');

    std::vector<float> SplitStrToFloatArray(const std::string& str,char delim= ' ');

    std::vector<int> SplitStrToIntArray(const std::string& str,char delim=' ');

    //JAVA��Trim
    std::string TrimStr(std::string input);

    //Utf8�ַ�����
    int Utf8Strlen(const std::string& str);

    /*
     *������ʾ������ģ��Ի�������������ʾ�����ֻ�Ч����
     *�Ի�����һ��Ϊline����ʾ��posλ�õ��ַ��������������С�pos�Ժ�Ĳ���ʾ
     */
    std::vector<std::string> SplitByUTF8CharPos(const std::string& input,
                                            int line,int pos);
    //JAVA��ReplaceAll
    void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

    //�����Ӵ�
    bool Contains(const std::string& original,std::string sub);

    std::vector<std::string> SearchPattern(const std::string& str,const std::regex reg);

}


#endif // STR_UTILITIES_H_INCLUDED
