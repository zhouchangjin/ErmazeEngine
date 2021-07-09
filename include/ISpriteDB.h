#ifndef ISPRITEDB_H
#define ISPRITEDB_H

#include <CSprite.h>

class ISpriteDB
{
    public:
        ISpriteDB();
        virtual ~ISpriteDB();
        virtual void AddSpriteSheet(CSpriteSheet* sheet,std::string sheet_name)=0;
        virtual void AddSprite(std::string sprite_name,std::string sheet_name)=0;
        virtual void AddSpriteAction(std::string sprite_name,std::string action_name,std::vector<int>ids)=0;
        virtual CSprite* GetSprite(std::string sprite_name)=0;

    protected:

    private:
};

#endif // ISPRITEDB_H
