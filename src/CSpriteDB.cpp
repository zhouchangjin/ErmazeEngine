#include "CSpriteDB.h"

CSpriteDB::CSpriteDB()
{
    //ctor
}

CSpriteDB::~CSpriteDB()
{
    //dtor
    std::map<std::string,CSprite*>::iterator itc;
    for(itc=m_sprites.begin(); itc!=m_sprites.end(); itc++)
    {
        CSprite* sprite=itc->second;
        delete sprite;
    }
    m_sprites.clear();

    std::map<std::string,CSpriteSheet*>::iterator it;
    for(it=m_spritesheets.begin(); it!=m_spritesheets.end(); it++)
    {
        CSpriteSheet* sheet=it->second;
        delete sheet;
    }
    m_spritesheets.clear();
}

void CSpriteDB::AddSpriteSheet(CSpriteSheet* sheet,std::string sheetname)
{
    m_spritesheets[sheetname]=sheet;
}

bool CSpriteDB::ContainsSprite(std::string sprite_name){
    if(m_sprites.find(sprite_name)!=m_sprites.end()){
        return true;
    }else{
        return false;
    }
}

void CSpriteDB::AddSprite(std::string sprite_name,std::string sheet_name)
{
    bool flag=ContainsSprite(sprite_name);
    if(flag){
        return;
    }
    CSpriteSheet* sheet=m_spritesheets[sheet_name];
    if(sheet)
    {
        CSprite* sprite=new CSprite(sheet);
        m_sprites[sprite_name]=sprite;
    }
}

void CSpriteDB::AddSpriteAction(std::string sprite_name,
                                std::string action_name,std::vector<int> ids)
{
    CSprite* sprite=GetSprite(sprite_name);
    if(sprite)
    {
        sprite->AddAction(action_name,ids);
    }
}

CSprite* CSpriteDB::GetSprite(std::string sprite_name)
{
    if(m_sprites.find(sprite_name)!=m_sprites.end())
    {
        return m_sprites[sprite_name];
    }
    else
    {
        return nullptr;
    }
}

void CSpriteDB::Initialize()
{

    xmlutils::MyXMLDoc doc=xmlutils::LoadXML("./sprites/sprites_db.xml");
    xmlutils::MyXMLNode sheet_node=doc.
                                   GetNode("/spritesdb/spritesheets");
    xmlutils::MyXMLNode sprite_node=doc.GetNode("/spritesdb/sprites");

    std::map<std::string,ge_common_struct::image_def> sheets;
    std::map<std::string,ge_common_struct::image_def>::iterator it_sheet;

    std::vector<ge_common_struct::sprite_action> actions;


    ge_fileutil::parse_sheets(sheet_node,sheets);
    ge_fileutil::parse_sprites_action(sprite_node,actions);

    for(it_sheet=sheets.begin(); it_sheet!=sheets.end(); it_sheet++)
    {
        ge_common_struct::image_def image=it_sheet->second;
        CSpriteSheet* sprite_sheet=new CSpriteSheet(image.path,image.width,
                image.height,
                image.col,image.row);
        AddSpriteSheet(sprite_sheet,image.id);
    }

    for(size_t i=0;i<actions.size();i++){
        ge_common_struct::sprite_action action=actions[i];
        std::string sprite_name=action.sprite_id;
        std::string sheet_name=action.sheet_id;
        if(!ContainsSprite(sprite_name)){
            AddSprite(sprite_name,sheet_name);
        }
        AddSpriteAction(sprite_name,action.action_name,action.ids);
    }

}
