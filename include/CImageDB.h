#ifndef CIMAGEDB_H
#define CIMAGEDB_H
#include <map>
#include <string>
#include <vector>
#include <CIcon.h>

class CImageDB
{
    public:
        CImageDB();
        virtual ~CImageDB();
        void AddSpriteSheet(CSpriteSheet* sheet,std::string sheet_name);
        void AddIconSheet(std::string sheet_name,std::string icon_name,int icon_idx);
        CIcon GetIcon(std::string icon_name);
        CTiledIcon GetTiledIcon(std::string icon_name);
        bool ContainsSheet(std::string sheet_name);
        bool ContainsIcon(std::string icon_name);
    protected:
        std::vector<CSpriteSheet*> m_spritesheets;
        std::map<std::string,CTiledIcon> m_icons;
        std::map<std::string,std::string> m_icon_map;
        std::map<std::string,int> m_sheet_map;

    private:
};

#endif // CIMAGEDB_H
