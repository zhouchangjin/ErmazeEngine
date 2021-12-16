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
        void SetRenderScale(int scale){m_render_scale=scale;};
        int GetLayer()const{return m_layer;};
        int GetShowLayer();
        int GetFootY()const{return m_y+GetObjectHeight()-1;}
        int GetMoveX()const{return m_move_x;};
        int GetMoveY()const{return m_move_y;};
        int GetMoveSpeed()const{return m_move_speed;};
        int GetObjectWidth()const{return m_sprite->
        GetSpriteSheet()->GetSpriteWidth();};
        int GetObjectHeight()const{return m_sprite->GetSpriteSheet()->GetSpriteHeight();};
        int GetFrameIdx()const{return m_frame_idx;};
        int GetRenderScale()const{return m_render_scale;};
        CSprite* GetSprite(){return m_sprite;};
        void AddActionLog(ge_common_struct::action_type log);
        void ClearActionLog();
        void ClearMove(){m_move_x=0;m_move_y=0;};
        void Step(){m_step++;};
        int GetStep(){return m_step;};
        void ResetStep(){m_step=0;};
        bool CheckCollision(const CSpriteGameObject& other);
        std::string GetCurrentAction()const{return m_current_action;};
        int GetCurrentOrientation()const;

        bool IsEnableTransparency()const{return m_enable_transparency;};
        void SetTransparency(bool enable){m_enable_transparency=enable;};
        float GetAlpha()const{return m_alpha;};
        void SetAlpha(float alpha){m_alpha=alpha;};
        void EnableMask(){m_enable_mask=true;};
        void DisableMask(){m_enable_mask=false;};
        void EnableOutline(){m_enable_outline=true;};
        void DisableOutline(){m_enable_outline=false;};
        bool IsEnableMask(){return m_enable_mask;};
        bool IsEnableOutline(){return m_enable_outline;};

        void SetOutlineColor(ge_common_struct::ge_adv_color c){m_outline_color=c;};
        void SetMaskColor(ge_common_struct::ge_adv_color c){m_mask_color=c;};
        ge_common_struct::ge_adv_color GetOutlineColor()const{return m_outline_color;};
        ge_common_struct::ge_adv_color GetMaskColor()const{return m_mask_color;};

    protected:
        CSprite* m_sprite;
        int m_frame_idx=0;
        int m_step=0;
        std::string m_current_action="downward";
        int m_move_x=0;
        int m_move_y=0;
        int m_move_speed=8;

        //####显示层级相关属性
        //当前层级
        int m_layer=1;
        //当前显示层级
        int m_show_layer=1;
        //是否在阶梯之上
        bool m_onstair=false;

        //####显示相关属性
        //渲染尺寸
        int m_render_scale=1;
        //渲染生命(透明度)
        float m_alpha=1.0f;

        ge_common_struct::ge_adv_color m_outline_color;

        ge_common_struct::ge_adv_color m_mask_color;

        bool m_enable_outline=false;

        bool m_enable_mask=false;

        bool m_enable_transparency=true;


        //存储动作历史记录栈大小（应该与人数相同）
        unsigned int m_pop_size=2;
        std::queue<ge_common_struct::action_type> m_action_log;

        CCamera2D* m_camera=nullptr;
        CSpriteGameObject* m_pal=nullptr;

    private:

};

#endif // CSPRITEGAMEOBJECT_H
