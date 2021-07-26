#include <File_Util.h>

namespace ge_fileutil
{

void CBinaryFileReader::ReOpen()
{
    Close();
    m_istream=std::ifstream(m_file_name,std::ios::binary);
}

bool CBinaryFileReader::EndOfFile(){
    int c=m_istream.peek();
    if(c==EOF){
        return true;
    }else{
        return false;
    }
}

void CBinaryFileReader::Close()
{

    if(m_istream.is_open())
    {
        m_istream.close();
    }
}

void CBinaryFileReader::ReadInt(int& num)
{
    m_istream.read((char*)&num,sizeof(num));
}

void CBinaryFileReader::ReadFloat(float& flt)
{
    m_istream.read((char*)&flt,sizeof(flt));

}

void CBinaryFileReader::ReadDouble(double& dbl)
{
    m_istream.read((char*)&dbl,sizeof(dbl));
}

void CBinaryFileReader::ReadStringWithFixSize(std::string& str,int fixsize)
{

    char buff[fixsize+1];
    m_istream.read(buff,fixsize);
    buff[fixsize]='\0';
    str=ge_str_utilities::TrimStr(std::string(buff));
}

void parse_chunk_file_bydef(CBinaryFileReader& infile,chunk& chunk_def,IChunkFactory* factory)
{

    chunk_type type=chunk_def.type;
    std::string chunk_name=chunk_def.chunk_name;
    factory->InitializeChunk(chunk_name);
    if(type==chunk_type::CHUNK)
    {
        if(chunk_def.is_list)
        {
            uint32_t chunk_size=chunk_def.chunk_size;
            //确定list的结尾
            if(chunk_def.variable_size)
            {
                std::string ref_chunk=chunk_def.ref_chunk;
                //get size reference;
                chunk_size=factory->CalListSize(ref_chunk);
            }
            if(chunk_size>0)
            {
                for(size_t i=0; i<chunk_size; i++)
                {
                    for(size_t j=0; j<chunk_def.chunk_list_entity.size(); j++)
                    {
                        chunk element=chunk_def.chunk_list_entity[j];
                        parse_chunk_file_bydef(infile,element,factory);
                    }

                }

            }
            else
            {
                //直接到文件末尾
                while(!infile.EndOfFile())
                {
                    for(size_t j=0; j<chunk_def.chunk_list_entity.size(); j++)
                    {
                        chunk element=chunk_def.chunk_list_entity[j];
                        parse_chunk_file_bydef(infile,element,factory);
                    }
                }

            }

        }
        else
        {
            for(size_t i=0; i<chunk_def.chunk_properties.size(); i++)
            {

                chunk prop_def=chunk_def.chunk_properties[i];
                parse_chunk_file_bydef(infile,prop_def,factory);
            }
        }

    }
    //不考虑list目前
    else if(type==chunk_type::STRING)
    {
        std::string readstr;
        infile.ReadStringWithFixSize(readstr,chunk_def.chunk_size);
        factory->SetStrProperty(chunk_name,readstr);
    }
    else if(type==chunk_type::INT)
    {
        if(chunk_def.is_list)
        {

        }
        else
        {
            int number;
            infile.ReadInt(number);
            factory->SetIntProperty(chunk_name,number);
        }

    }
    else if(type==chunk_type::FLOAT)
    {
        if(chunk_def.is_list)
        {

        }
        else
        {
            float decimal;
            infile.ReadFloat(decimal);
            factory->SetFloatProperty(chunk_name,decimal);
        }

    }
    else if(type==chunk_type::DOUBLE)
    {
        if(chunk_def.is_list)
        {

        }
        else
        {
            double decimal;
            infile.ReadDouble(decimal);
            factory->SetDoubleProperty(chunk_name,decimal);
        }


    }
    else if(type==chunk_type::CHAR)
    {


    }
    else if(type==chunk_type::BYTE)
    {

    }
    else if(type==chunk_type::UNKNOWN_CHUNK)
    {
        std::string val=factory->GetRefObjVal(chunk_name,chunk_def.ref_chunk);
        chunk_type type=chunk_def.rule[val];
        int sizen=chunk_def.chunk_size_rule[val];
        if(type==chunk_type::INT)
        {
            if(chunk_def.is_list)
            {

            }
            else
            {
                int number;
                infile.ReadInt(number);
                factory->SetIntProperty(chunk_name,number);
            }
        }
        else if(type==chunk_type::DOUBLE)
        {
            if(chunk_def.is_list)
            {

            }
            else
            {
                double decimal;
                infile.ReadDouble(decimal);
                factory->SetDoubleProperty(chunk_name,decimal);
            }

        }
        else if(type==chunk_type::FLOAT)
        {
            if(chunk_def.is_list)
            {

            }
            else
            {
                float decimal;
                infile.ReadFloat(decimal);
                factory->SetFloatProperty(chunk_name,decimal);
            }
        }else if(type==chunk_type::STRING){
            std::string readstr;
            infile.ReadStringWithFixSize(readstr,sizen);
            factory->SetStrProperty(chunk_name,readstr);
        }
    }
    else
    {


    }

}

}
