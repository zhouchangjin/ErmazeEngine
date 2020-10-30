#ifndef CSPRITEGAMEOBJECT_H
#define CSPRITEGAMEOBJECT_H

#include <CGameObject.h>
#include <CSprite.h>
#include <CCamera2D.h>

class CSpriteGameObject  : public CGameObject
{
    public:
        CSpriteGameObject(CSprite* sprite);
        virtual ~CSpriteGameObject();
        void PlayAction(std::string action_name);
        void Play();
        void BindCamera(CCamera2D* camera);
        void BindPal(CSpriteGameObject* pal);
        void UpdateX(int x);
        void UpdateY(int y);
        void UpdateXY(int x,int y);
        void UpdateDirection(std::string action_name);
        void UpdateLayer(int layer);
        void UpdateShowLayer(int layer);
        void SetOnStair(bool onstair){m_onstair=onstair;};
        bool IsOnStair(){return m_onstair;};
        void MoveUpward();
        void MoveDownward();
        void MoveLeftward();
        void MoveRightward();
        void MoveUpward(int dy);
        void MoveDownward(int dy);
        void MoveLeftward(int dy);
        void MoveRightward(int dy);
        bool IsMoving();
        void StopMoving();
        void MoveUpdate();
        int GetLayer(){return m_layer;};
        int GetShowLayer();
        int GetX(){return m_x;};
        int GetY(){return m_y;};
        int GetMoveX(){return m_move_x;};
        int GetMoveY(){return m_move_y;};
        int GetMoveSpeed(){return m_move_speed;};
        int GetFrameIdx(){return m_frame_idx;};
        CSprite* GetSprite(){return m_sprite;};
        void AddActionLog(ge_common_struct::action_type log);
    protected:

    private:
        CSprite* m_sprite;
        int m_frame_idx=0;
        int m_step=0;
        int m_x=0;
        int m_y=0;
        std::string m_current_action="downward";
        int m_move_x=0;
        int m_move_y=0;
        int m_move_speed=8;
        int m_layer=1;
        int m_show_layer=1;
        bool m_onstair=false;
        unsigned int m_pop_size=2;
        std::queue<ge_common_struct::action_type> m_action_log;

        CCamera2D* m_camera=nullptr;
        CSpriteGameObject* m_pal=nullptr;

        void Step(){m_step++;};

};

#endif // CSPRITEGAMEOBJECT_H
