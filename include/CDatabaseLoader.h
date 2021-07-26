#ifndef CDATABASELOADER_H
#define CDATABASELOADER_H

#include "CGameDataChunkFactory.h"
#include "CGameDatabase.h"

class CDatabaseLoader
{
    public:
        /** Default constructor */
        CDatabaseLoader();
        CDatabaseLoader(CGameDatabase* database){m_gamedatabase=database;};
        /** Default destructor */
        virtual ~CDatabaseLoader();
        std::string GetObjectNameProperty()const{return m_object_name_property;};
        std::string GetObjectLabelProperty()const{return m_object_label_property;};
        void SetObjectNameProperty(std::string name){m_object_name_property=name;};
        void SetObjectLabelProperty(std::string label){m_object_label_property=label;};
        void SetGameDatabase(CGameDatabase* database){m_gamedatabase=database;};
        void LoadObjects(CGameDataChunkFactory* factory);
    protected:
        std::string m_object_name_property="name";
        std::string m_object_label_property="label";
        CGameDatabase* m_gamedatabase;
    private:
};

#endif // CDATABASELOADER_H
