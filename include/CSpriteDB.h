#ifndef CSPRITEDB_H
#define CSPRITEDB_H

#include <ISpriteDB.h>
#include <XML_Utilities.h>
#include <GameFileLoader.h>

class CSpriteDB : public ISpriteDB
{
    public:
        CSpriteDB();
        virtual ~CSpriteDB();
        void AddSpriteSheet(CSpriteSheet* sheet,std::string sheet_name);
        void AddSprite(std::string sprite_name,std::string sheet_name);
        void AddSpriteAction(std::string sprite_name,std::string action_name,std::vector<int>ids);
        CSprite* GetSprite(std::string sprite_name);
        bool ContainsSprite(std::string sprite_name);
        void Initialize();

    protected:
        std::map<std::string,CSpriteSheet*> m_spritesheets;
        std::map<std::string,CSprite*> m_sprites;
        std::map<std::string,std::string> m_spritesheet_map;

    private:
};

#endif // CSPRITEDB_H
