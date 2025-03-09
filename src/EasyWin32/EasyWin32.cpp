#include "EasyWin32.hpp"
#include "EApplication.hpp"

#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include <iostream>
#include <sstream>

int _USER_MAIN_ENTRY(int argc, char* argv[],HINSTANCE hInst,HINSTANCE hInstPrev,PSTR cmdline,int cmdshow);

#ifdef WinMain
#undef WinMain
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    int ret = 0;
    try{
        ret = _USER_MAIN_ENTRY(__argc, __argv,hInst,hInstPrev,cmdline,cmdshow);
    }catch(const std::exception& e){
        std::wstringstream wss;
        wss << L"初始化错误！\n请检查入口函数内的初始化代码(不包含回调函数)\n错误信息:\n"<<e.what();
        return MessageBoxW(NULL,wss.str().c_str(),L"EasyWin32初始化错误!",MB_OK|MB_ICONERROR);
    }
    MSG msg;
    try{
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }catch(const std::exception& e){
        WCHAR title[256];
        GetWindowTextW(msg.hwnd,title,256);
        std::wstringstream wss;
        wss << L"事件过程错误！请检查回调函数\n窗口标题:"<<title<<L"  事件消息值:"<<msg.message<<L"(Windows消息)\n错误信息:\n"<<e.what();
        return MessageBoxW(msg.hwnd,wss.str().c_str(),L"EasyWin32事件过程错误!",MB_OK|MB_ICONERROR);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return ret;
}
#endif

#ifdef main
#undef main
int main(int argc, char* argv[]){
    EApplication->_setIsWinMainEntry(FALSE);
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\033[1;33m";//黄色
    std::cout << "[WARN] 当前以控制台模式运行，请为编译器添加WIN32标记以启用GUI模式(无控制台)";
    std::cout << "\033[0m";
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWDEFAULT);
}
#endif