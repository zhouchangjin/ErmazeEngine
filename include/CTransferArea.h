#ifndef CTRANSFERAREA_H
#define CTRANSFERAREA_H
#include <common_struct.h>

class CTransferArea
{
public:
    CTransferArea();
    CTransferArea(const CTransferArea& copyarea);
    virtual ~CTransferArea();
    int GetX()const
    {
        return m_x;
    };
    int GetY()const
    {
        return m_y;
    };
    int GetWidth()const
    {
        return m_width;
    };
    int GetHeight()const
    {
        return m_height;
    };
    int GetMapX()const
    {
        return m_map_x;
    };
    int GetMapY()const
    {
        return m_map_y;
    };
    int GetMapLayer()const
    {
        return m_map_layer;
    };
    std::string GetScene()const
    {
        return m_scene;
    };
    ge_common_struct::action_type GetDirection()const
    {
        return m_direction;
    };

    void SetX(int x)
    {
        m_x=x;
    };
    void SetY(int y)
    {
        m_y=y;
    };
    void SetWidth(int width)
    {
        m_width=width;
    };
    void SetHeight(int height)
    {
        m_height=height;
    };
    void SetMapX(int mapx)
    {
        m_map_x=mapx;
    };
    void SetMapY(int mapy)
    {
        m_map_y=mapy;
    };
    void SetMapLayer(int maplayer)
    {
        m_map_layer=maplayer;
    };
    void SetScene(std::string scene)
    {
        m_scene=scene;
    };
    void SetDirection(ge_common_struct::action_type action)
    {
        m_direction=action;
    };

protected:

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_map_x;
    int m_map_y;
    int m_map_layer;
    std::string m_scene;
    ge_common_struct::action_type m_direction;

};

#endif // CTRANSFERAREA_H
