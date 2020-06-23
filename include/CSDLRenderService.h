#ifndef CSDLRENDERSERVICE_H
#define CSDLRENDERSERVICE_H

#include "SDL_Utilities.h"
#include "common_struct.h"

/**
未完成 暂时弃用  Service的方式实现比较麻烦
用于维护SDL相关对象（依赖注入） 可以选择service locator模式 单例  全局对象 普通依赖注入
**/
class CSDLRenderService
{
    public:
        CSDLRenderService();
        virtual ~CSDLRenderService();

    protected:

    private:
};

#endif // CSDLRENDERSERVICE_H
