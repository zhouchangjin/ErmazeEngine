#include "Str_Utilities.h"
namespace ge_str_utilities
{
std::string TrimStr(std::string input)
{
    std::string::size_type start_pos = input.find_first_not_of(' ');
    std::string::size_type end_pos = input.find_last_not_of(' ');
    start_pos=(start_pos == std::string::npos ? input.length(): start_pos);
    end_pos=(end_pos == std::string::npos? input.length()-1: end_pos);
    if(start_pos<input.length()){
        std::string res = input.substr(start_pos,end_pos - start_pos + 1);
        return res;
    }else{
        return "";
    }

}

std::vector<std::string> SplitByUTF8CharPos(const std::string& input,
        int line,int pos)
{
    std::vector<std::string> lines;
    size_t ps=0;
    int cCnt=0;
    size_t cSize=input.length();
    for(; ps<cSize; cCnt++)
    {
        if(cCnt>pos)
        {
            break;
        }
        int col=cCnt%line;
        int row=cCnt/line;
        if(col==0)
        {
            lines.push_back("");
        }
        int c=(unsigned char)input[ps];
        if(c>=0 && c<=127)
        {
            lines[row]+=input.substr(ps,1);
            ps++;
        }
        else if((c & 0xE0) == 0xC0)
        {
            lines[row]+=input.substr(ps,2);
            ps+=2;
        }
        else if ((c & 0xF0) == 0xE0)
        {
            lines[row]+=input.substr(ps,3);
            ps+=3;
        }
        else if ((c & 0xF8) == 0xF0)
        {
            lines[row]+=input.substr(ps,4);
            ps+=4;
        }
        else
        {
            return lines;
        }
    }
    return lines;
}

std::vector<std::string> SplitStr(const std::string& input,char delim)
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

int Utf8Strlen(const std::string& str)
{
    int c,i,ix,q;
    for (q=0, i=0, ix=str.length(); i < ix; i++, q++)
    {
        c = (unsigned char) str[i];
        if      (c>=0   && c<=127) i+=0;
        else if ((c & 0xE0) == 0xC0) i+=1;
        else if ((c & 0xF0) == 0xE0) i+=2;
        else if ((c & 0xF8) == 0xF0) i+=3;
        else return 0;//invalid utf8
    }
    return q;

}

void ReplaceAll(std::string& str, const std::string& from,
                const std::string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

bool Contains(const std::string& original,std::string sub)
{
    if(original.find(sub)!=std::string::npos){

        return true;
    }else{
        return false;
    }
}

std::vector<std::string> SearchPattern(const std::string& str,
                                       const std::regex reg){
    std::vector<std::string> result;
    std::smatch mr;
    std::string search_str=str;
    while(std::regex_search(search_str,mr,reg)){
        result.push_back(mr[0]);
        search_str=mr.suffix().str();
    }
    return result;

}

}
