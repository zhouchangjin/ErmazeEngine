#include "Str_Utilities.h"
namespace ge_str_utilities
{
std::string TrimStr(std::string input)
{
    std::string::size_type start_pos = input.find_first_not_of(' ');
    std::string::size_type end_pos = input.find_last_not_of(' ');
    std::string res = input.substr(start_pos == std::string::npos ? 0 : start_pos,end_pos == std::string::npos ? input.length() - 1 : end_pos - start_pos + 1);
    return res;
}

std::vector<std::string> Splitstr(const std::string& input,char delim)
{
    std::string str=TrimStr(input);
    std::vector<std::string> splitarray;
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos)
    {
        splitarray.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    splitarray.push_back(str.substr(previous, current - previous));
    return splitarray;
}

std::vector<float> SplitStrToFloatArray(const std::string& input,char delim)
{
    std::string str=TrimStr(input);
    std::vector<float> splitarray;
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos)
    {
        splitarray.push_back(atof(str.substr(previous, current - previous).c_str()));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    splitarray.push_back(atof(str.substr(previous, current - previous).c_str()));
    return splitarray;
}

std::vector<int> SplitStrToIntArray(const std::string& input,char delim)
{
    std::string str=TrimStr(input);
    std::vector<int> splitarray;
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos)
    {
        splitarray.push_back(atoi(str.substr(previous, current - previous).c_str()));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    splitarray.push_back(atoi(str.substr(previous, current - previous).c_str()));
    return splitarray;
}

}
