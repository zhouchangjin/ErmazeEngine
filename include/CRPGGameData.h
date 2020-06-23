#ifndef CRPGGAMEDATA_H
#define CRPGGAMEDATA_H

#include <CGameData.h>


class CRPGGameData : public CGameData
{
    public:
        CRPGGameData();
        virtual ~CRPGGameData();

        int GetX(){return m_coor_x;};
        int GetY(){return m_coor_y;};
        int GetScene(){return m_scene_id;};


    protected:

    private:
        int m_coor_x=0;
        int m_coor_y=0;
        int m_scene_id=1;
};

#endif // CRPGGAMEDATA_H
