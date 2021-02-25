#include "hook.h"
#include<QDebug>
#include"qprocess.h"
#ifdef Q_OS_WIN
Hook::Hook()
{

}

static HHOOK keyHook=nullptr;
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    KBDLLHOOKSTRUCT *pkbhs=(KBDLLHOOKSTRUCT*)lParam;
//    qDebug()<<"code:"<<nCode;
//    qDebug()<<"flag:"<<pkbhs->flags;
//    qDebug()<<"vkCode:"<<pkbhs->vkCode;
//    qDebug()<<"scanCode:"<<pkbhs->scanCode;
    //if(nCode == HC_ACTION)
    if(nCode>=0)
    {
        if(pkbhs->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL)& 0x8000 && GetAsyncKeyState(VK_SHIFT)&0x8000)
        {
           qDebug() << "Ctrl+Shift+Esc";
            return 1;
        }
        else if(pkbhs->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL) & 0x8000)
        {
            qDebug() << "Ctrl+Esc";
             return 1;
        }
        else if(pkbhs->vkCode == VK_DELETE && VK_MENU && VK_CONTROL)
        {

             qDebug() << "Alt+Delete+ctrl";
             return 1;
        }
        else if(pkbhs->vkCode == VK_DELETE && GetAsyncKeyState(VK_CONTROL) & 0x8000  && (pkbhs->flags &LLKHF_ALTDOWN))
        {
            qDebug() << "Alt+Ctrl+Delete";
             return 1;
        }
        else if(pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
        {
            qDebug() << "Alt+Tab";
            return 1;
        }
        else if(pkbhs->vkCode == VK_ESCAPE && pkbhs->flags &LLKHF_ALTDOWN)
        {
            qDebug() << "Alt+Esc";
             return 1;
        }
        else if(pkbhs->vkCode == VK_LWIN || pkbhs->vkCode == VK_RWIN)
        {
            qDebug() << "LWIN/RWIN";
             return 1;
        }
        else if(pkbhs->vkCode == VK_F4 && pkbhs->flags & LLKHF_ALTDOWN)
        {
            qDebug() << "Alt+F4";
            return 1;
        }
        else if(pkbhs->vkCode==VK_TAB &&GetAsyncKeyState(VK_CONTROL) & 0x8000)
        {
            qDebug()<<"Ctrl+TAB";
            return 1;
        }
//        else if(pkbhs->vkCode==65);
//        {
//            qDebug()<<"A";
//            return 1;
//        }
        return 0;//返回1表示截取消息不再传递,返回0表示不作处理,消息继续传递
    }
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

void Hook::unInstallHook()
{
    if(keyHook!=nullptr)
    {
        UnhookWindowsHookEx(keyHook);//键盘钩子句不为空时销毁掉
        keyHook=nullptr;
    }
    qDebug()<<"uninstall hook";
}

void Hook::installHook()
{
    //HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
    keyHook =SetWindowsHookEx( WH_KEYBOARD_LL,keyProc, GetModuleHandle(NULL),0);
    qDebug() << "keyHook:" << keyHook;
}
#endif
