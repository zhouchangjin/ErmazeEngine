#include "C2DGameScene.h"
#include <iostream>

C2DGameScene::C2DGameScene()
{
    //ctor
}

C2DGameScene::~C2DGameScene()
{

}

C2DGameScene::C2DGameScene(const C2DGameScene& copyscene)
{

}

void C2DGameScene::ClearScene()
{
    CGameScene::ClearScene();
    m_collidable_tiles.clear();
    m_layerswitches.clear();
    m_transfer_area.clear();
    m_grid_type.clear();

    std::map<std::string,CSprite*>::iterator it;
    for(it=m_sprites.begin(); it!=m_sprites.end(); it++)
    {
        CSprite* object=it->second;
        delete object;
        object=nullptr;
    }
    for(size_t i=0; i<m_npcs.size(); i++)
    {
        CNPCGameObject* npc_obj=m_npcs[i];
        ge_common_struct::dialog_tree_node* dn=npc_obj->GetDialogTree();
        delete dn;
        delete npc_obj;
        npc_obj=nullptr;
        m_npcs[i]=nullptr;
    }
    m_npcs.clear();
    m_sprites.clear();
}

void C2DGameScene::AddSprite(std::string sprite_id,CSpriteSheet* sheet)
{
    CSprite* sprite=new CSprite(sheet);
    m_sprites[sprite_id]=sprite;
}

void C2DGameScene::AddSpriteAction(std::string sprite_id,
                                   std::string action_name,
                                   std::vector<int> ids)
{
    m_sprites[sprite_id]->AddAction(action_name,ids);
}

CSprite* C2DGameScene::GetSprite(std::string sprite_id)
{
    return m_sprites[sprite_id];
}


bool C2DGameScene::GetTileCollideBool(int tile_id)
{
    if(m_collidable_tiles.find(tile_id)!=m_collidable_tiles.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool C2DGameScene::GetSwitchTileBool(int tile_id)
{
    if(m_layerswitches.find(tile_id)!=m_layerswitches.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void C2DGameScene::AddCollideTile(int tile_id)
{
    m_collidable_tiles[tile_id]=true;
}

void C2DGameScene::AddSwitchTile(int tile_id)
{
    m_layerswitches[tile_id]=true;
}

CTileLayer* C2DGameScene::GetTileLayer(int layer_no)
{

    CLayer* layer=GetLayer(layer_no);
    if(layer->GetLayerType()==ge_common_struct::layer_type::TILED_LAYER)
    {

        return (CTileLayer*) layer;

    }
    else
    {
        return NULL;
    }
}

void C2DGameScene::AddGridType(int gridx,int gridy,
                               ge_common_struct::grid_type type)
{
    int gridId=gridy*m_map_width+gridx;
    m_grid_type[gridId]=type;
}

ge_common_struct::grid_type C2DGameScene::GetGridType(int gridx,int gridy)
{
    int gridId=gridy*m_map_width+gridx;
    if(m_grid_type.find(gridId)!=m_grid_type.end())
    {
        return ge_common_struct::grid_type::NORMAL;
    }
    else
    {
        return m_grid_type[gridId];
    }
}

CNPCGameObject* C2DGameScene::CreateNpc(std::string sprite_id,int x,int y,
                             int layer,int direction)
{

    CSprite* sprite=GetSprite(sprite_id);
    CNPCGameObject* object=new CNPCGameObject(sprite);
    m_npcs.push_back(object);
    object->UpdateXY(x,y);
    object->UpdateLayer(layer);
    ge_common_struct::action_type act_type=(ge_common_struct::action_type)direction;
    if(act_type==ge_common_struct::action_type::MOVE_UP){
        object->UpdateDirection("upward");
    }else if(act_type==ge_common_struct::action_type::MOVE_DOWN){
        object->UpdateDirection("downward");
    }else if(act_type==ge_common_struct::action_type::MOVE_LEFT){
        object->UpdateDirection("leftward");
    }else if(act_type==ge_common_struct::action_type::MOVE_RIGHT){
        object->UpdateDirection("rightward");
    }
    return object;
}


ge_common_struct::ge_point C2DGameScene::GetGridCoor(int x_coor,int y_coor){
    ge_common_struct::ge_point point;
    point.x=x_coor/m_tile_width;
    point.y=y_coor/m_tile_height;
    return point;
}


