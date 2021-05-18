#ifndef CMENUINPUTEVENT_H
#define CMENUINPUTEVENT_H
#include <CInputEvent.h>


class CMenuInputEvent : public CInputEvent
{
    public:
        CMenuInputEvent();
        virtual ~CMenuInputEvent();
        std::string const GetMenuInitPanel(){return m_init_panel;};
        void setMenuInitPanel(std::string panel){m_init_panel=panel;};

    protected:
        std::string m_init_panel;

    private:
};

#endif // CMENUINPUTEVENT_H
