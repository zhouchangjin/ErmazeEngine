#ifndef ITEXTUREDB_H
#define ITEXTUREDB_H

#include <string>
#include <CTexture.h>
#include <CSpriteSheet.h>

class ITextureDB
{
    public:
        ITextureDB();
        virtual ~ITextureDB();
        virtual void AddSpriteSheet(CSpriteSheet* sheet,std::string sheet_name)=0;
        virtual void AddTexture(std::string sheet_name,std::string texture_name,int idx)=0;
        virtual CTexture GetTexture(std::string texture_name)=0;

    protected:

    private:
};

#endif // ITEXTUREDB_H
