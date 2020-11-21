#ifndef CADVDIALOG_H
#define CADVDIALOG_H
/**
�߼��Ի��򣬶�һ���ظ��Ĵ���
**/
#include <CGameDialog.h>
#include <CChoiceDialog.h>

class CAdvDialog : public CGameDialog
{
    public:
        CAdvDialog();
        virtual ~CAdvDialog();
        void SetResponseBackgroundColor(ge_common_struct::ge_adv_color color);
        void SetResponseBorderColor(ge_common_struct::ge_color color);
        void SetResponseWindowRect(ge_common_struct::ge_rect rect);
        void InitResponseByDialog();
        bool IsChoiceActive(){return m_choicebox_active;};
        void ShowChoice(){m_choicebox_active=true;};
        void HideChoice(){m_choicebox_active=false;};
        CChoiceDialog GetChoiceDialog()const{return m_choice_dialog;};
        void initChoice(std::vector<std::string> choices);
    protected:
        CChoiceDialog m_choice_dialog;
        bool m_choicebox_active=true;
    private:
};

#endif // CADVDIALOG_H
