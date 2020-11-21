#ifndef CMENUSTATE_H
#define CMENUSTATE_H

#include <CGameState.h>
#include <string>
#include "Render_Util.h"

class CMenuState : public CGameState
{
    public:
        CMenuState();
        CMenuState(CGameContext * context);
        virtual ~CMenuState();
        void Init();
        void Cleanup();
        void Pause();
        void Resume();
        void HandleEvent(ge_common_struct::input_event event);
        void Update();
        void Draw();
        void PrepareData(){};
        int GetStateValue();
    protected:

    private:
        std::string m_background_path="res/menu.png";
        std::string m_font_path="fonts/simhei.ttf";
        void* m_background_texture;
        void* m_font;
        ge_common_struct::ge_rect m_select_rect;
        int m_select_idx=0;

        std::string m_menu_1="Start Game";
        std::string m_menu_2="Load Game";
        std::string m_menu_3="Quit Game";
};

#endif // CMENUSTATE_H
