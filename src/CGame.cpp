#include "CGame.h"

CGame::CGame()
{
    //ctor
}

CGame::~CGame()
{
    //dtor
}

void CGame::Run(){




    //ge_fileutil::CBinaryFileReader file("c:/playerdata2.dat");
    //file.ReOpen();
    //CGameDataChunkFactory factory;
    //ge_fileutil::parse_chunk_file_bydef(file,chunk,&factory);
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
