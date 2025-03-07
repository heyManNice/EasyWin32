#include "EApplication.hpp"
#include "EWindow.hpp"

#include <windows.h>
#include <string>
#include <shellscalingapi.h>

EApplicationSingleton *EApplicationSingleton::instance = nullptr;

EApplicationSingleton *EApplicationSingleton::getInstance()
{
    if (instance == nullptr)
    {
        // 注册窗口类
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = &EWindow::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = L"EasyWin32ClassName";
        wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        RegisterClassExW(&wcex);
        instance = new EApplicationSingleton();
    }
    return instance;
}

int EApplicationSingleton::alert(const std::wstring &message, const std::wstring &caption, UINT type)
{
    return MessageBoxW(NULL, message.c_str(), caption.c_str(), type);
}

int EApplicationSingleton::quit()
{
    PostQuitMessage(0);
    return 0;
}
int EApplicationSingleton::getDPI()
{
    return this->dpi;
}

void EApplicationSingleton::_setIsWinMainEntry(BOOL value)
{
    this->isWinMainEntry = value;
}

BOOL EApplicationSingleton::getIsWinMainEntry()
{
    return this->isWinMainEntry;
}

EWindow *EApplicationSingleton::getEWindowByTitle(const std::wstring &title)
{
    for(auto it = this->children.begin(); it != this->children.end(); it++){
        if (auto window = dynamic_cast<EWindow*>(*it)){
            if (window->getTitle() == title){
                return window;
            } 
        }
    }
    return nullptr;
}

EWindow *EApplicationSingleton::getEWindowByHWND(HWND hwnd)
{
    for(auto it = this->children.begin(); it != this->children.end(); it++){
        if (auto window = dynamic_cast<EWindow*>(*it)){
            if (window->getHWND() == hwnd){
                return window;
            } 
        }
    }
    return nullptr;
}

void EApplicationSingleton::updateDpiFromMonitor()
{
    HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTONEAREST);
    UINT dpiX, dpiY;
    GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    this->dpi = dpiX;
}