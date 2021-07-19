#include "CGame.h"
CGame::CGame()
{
    //ctor
}

CGame::~CGame()
{
    //dtor
}
/**
void printChunk(ge_fileutil::chunk& chunk){

    GE_LOG("%s=%d==\n",chunk.chunk_name.c_str(),chunk.type);
    if(chunk.chunk_properties.size()>0){
        for(size_t i=0;i<chunk.chunk_properties.size();i++){
            printChunk(chunk.chunk_properties[i]);
        }
    }
    if(chunk.chunk_list_entity.size()>0){
        for(size_t i=0;i<chunk.chunk_list_entity.size();i++){
            printChunk(chunk.chunk_list_entity[i]);
        }
    }
}
**/
void CGame::Run(){

    /**
    std::string filepath="./data/metadata.xml";
    xmlutils::MyXMLDoc doc=xmlutils::LoadXML(filepath);
    xmlutils::MyXMLNode xml_node=doc.GetNode("/filedef/chunk");
    ge_fileutil::chunk chunk;
    ge_fileutil::parse_chunk(xml_node,chunk);
    **/
    /**
    ge_fileutil::chunk_loader<CTest,int> loader;
    loader.chunk_setter=CTest::SetP;
    CTest* t=new CTest();
    CTest t2;
    (t->*loader.chunk_setter)(109);
    (t2.*loader.chunk_setter)(106);
    GE_LOG("%d==%d===========================\n",t->GetP(),t2.GetP());
    **/
    m_engine.Init();

    while(m_engine.Running()){
        m_engine.HandleEvent();
        m_engine.Update();
        m_engine.Draw();
    }


}
