#ifndef CANIMATIONITEM_H
#define CANIMATIONITEM_H

#include <common_struct.h>
#include <CSpriteGameObject.h>
#include <IEmitter.h>

class CAnimationItem
{
    public:
        enum AnimateType{
            TEXT_MOTION,
            PARTICLE,
            PROJECTILE,
            MOVE_SPRITE,
            SHOW_VFX,
            VECTOR_ANIMATION,
            FLASH_SPRITE,
            SHINE_SPRITE,
            DISAPPEAR
        };
        CAnimationItem();
        virtual ~CAnimationItem();

        void SetAnimateType(AnimateType animate_type){m_animate_type=animate_type;};
        void SetObject(CSpriteGameObject* object){m_object=object;};
        void SetActionName(std::string action_name){m_action_name=action_name;};
        void SetAnimateName(std::string aniname){m_animation_name=aniname;};
        void SetStartLoc(ge_common_struct::ge_point p){m_start_location=p;};
        void SetEndLoc(ge_common_struct::ge_point p){m_end_location=p;};
        void SetStartFrame(int frame){m_start_frame=frame;};
        void SetEndFrame(int frame){m_end_frame=frame;};
        void SetShowGhost(bool show){m_show_ghost=show;};
        void SetGlobalFrame(int frame){m_global_frame=frame;};
        void SetEmitter(IEmitter* emt){m_emitter=emt;};
        void SetSpriteName(std::string name){m_sprite_name=name;};
        void SetVfxSprite(CSprite* sprite){m_vfx_sprite=sprite;};
        void SetTargetObject(CSpriteGameObject* obj){m_target_object=obj;};
        void SetResetPosition(bool reset){m_reset_position=reset;};
        void SetText(std::string text){m_text=text;};
        void SetFontSize(int fsize){m_font_size=fsize;};
        void SetFontColor(ge_common_struct::ge_color color){m_font_color=color;};
        void SetFrameRate(int frame){m_frames_rate=frame;};
        void SetLoop(bool loop){m_loop_action=loop;};

        std::string GetAnimationName()const{return m_animation_name;};
        CSpriteGameObject* GetObject(){return m_object;};
        std::string GetActionName()const{return m_action_name;};
        AnimateType GetAnimateType()const{return m_animate_type;};
        ge_common_struct::ge_point GetStartLoc()const{return m_start_location;};
        ge_common_struct::ge_point GetEndLoc()const{return m_end_location;};
        int GetStartFrame()const{return m_start_frame;};
        int GetEndFrame()const{return m_end_frame;};
        bool IsShowGhost()const{return m_show_ghost;};
        int GetGlobalFrame()const{return m_global_frame;};
        IEmitter* GetEmitter(){return m_emitter;};
        std::string GetSpriteName(){return m_sprite_name;};
        CSprite* GetVfxSprite(){return m_vfx_sprite;};
        CSpriteGameObject* GetTargetObject(){return m_target_object;};
        bool GetResetPosition()const{return m_reset_position;};
        std::string GetText()const{return m_text;};
        int GetFontSize()const{return m_font_size;};
        ge_common_struct::ge_color GetFontColor(){return m_font_color;};
        int GetFrameRate()const{return m_frames_rate;};
        bool IsLoop()const{return m_loop_action;};

    protected:
        int m_global_frame;
        std::string m_animation_name;
        CSpriteGameObject* m_object=nullptr;
        CSpriteGameObject* m_target_object=nullptr;
        CSprite* m_vfx_sprite;
        IEmitter* m_emitter;
        std::string m_sprite_name;
        std::string m_action_name;
        AnimateType m_animate_type;
        ge_common_struct::ge_point m_start_location;
        ge_common_struct::ge_point m_end_location;
        int m_start_frame;
        int m_end_frame;
        int m_frames_rate=1;
        bool m_show_ghost=false;
        bool m_reset_position=false;
        bool m_loop_action=true;
        std::string m_text;
        int m_font_size;
        ge_common_struct::ge_color m_font_color;


    private:
};

#endif // ANIMATIONITEM_H
