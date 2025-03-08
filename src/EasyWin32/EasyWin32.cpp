#include "EasyWin32.hpp"
#include "EApplication.hpp"

#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include <iostream>

int _USER_MAIN_ENTRY(int argc, char* argv[],HINSTANCE hInst,HINSTANCE hInstPrev,PSTR cmdline,int cmdshow);

#ifdef WinMain
#undef WinMain
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    int ret = _USER_MAIN_ENTRY(__argc, __argv,hInst,hInstPrev,cmdline,cmdshow);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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