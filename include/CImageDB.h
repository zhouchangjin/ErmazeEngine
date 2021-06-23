#ifndef CIMAGEDB_H
#define CIMAGEDB_H
#include <map>
#include <string>
#include <vector>

#include <XML_Utilities.h>
#include <GameFileLoader.h>
#include <ITextureDB.h>

class CImageDB :public ITextureDB
{
    public:
        CImageDB();
        virtual ~CImageDB();
        void AddSpriteSheet(CSpriteSheet* sheet,std::string sheet_name);
        void AddTexture(std::string sheet_name,std::string texture_name,int idx);
        CTexture GetTexture(std::string texture_name);
        CTiledTexture GetTiledTexture(std::string texture_name);
        CTiledTexture GetTiledTextureBySheetName(std::string sheet_name);
        bool ContainsSheet(std::string sheet_name);
        bool ContainsTexture(std::string texture_name);
        void Initialize();
    protected:
        std::vector<CSpriteSheet*> m_spritesheets;
        std::map<std::string,CTiledTexture> m_textures;
        std::map<std::string,std::string> m_texture_map;
        std::map<std::string,int> m_sheet_map;
        void ResInit(std::map<std::string,ge_common_struct::resource_def> resource);

    private:
};

#endif // CIMAGEDB_H
