#include "EApplication.hpp"
#include "EWindow.hpp"

#include <windows.h>
#include <string>
#include <shellscalingapi.h>

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")



EApplicationSingleton *EApplicationSingleton::instance = nullptr;

EApplicationSingleton *EApplicationSingleton::getInstance()
{
    if (instance == nullptr)
    {
        // 注册窗口类
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = &EApplicationSingleton::WndProc;
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

EWindow* EApplicationSingleton::getEWindowByTitle(const std::wstring &title)
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

EWindow* EApplicationSingleton::getEWindowByHWND(HWND hwnd)
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

EWindow& EApplicationSingleton::createWindow(){
    auto window = new EWindow();
    return *window;
}

void EApplicationSingleton::updateDpiFromMonitor()
{
    HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTONEAREST);
    UINT dpiX, dpiY;
    GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    this->dpi = dpiX;
}


LRESULT CALLBACK EApplicationSingleton::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    // 系统主题改变时更新窗口主题色
    case WM_SETTINGCHANGE:
    {
        auto window = EApplication.getEWindowByHWND(hwnd);
        if (window != nullptr)
        {
            window->updateThemeMode();
        }
        // 更新dpi
        EApplication.updateDpiFromMonitor();
        break;
    }

    case WM_PAINT:
    {
        // 初始化
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        int dpi = EApplication.getDPI();
        double scale = (double)dpi / 96;
        graphics.ScaleTransform(scale, scale);

        auto layout = EApplication.getEWindowByHWND(hwnd)->getLayout();
        
        for(ELayout& child: layout.getChildren()){
            if(!child.text.empty()){
                Gdiplus::FontFamily fontFamily(L"HarmonyOS Sans sC");
                Gdiplus::Font font(&fontFamily, 30, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
                Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 0, 0, 0));
                Gdiplus::PointF pointF(child.x, child.y);
                graphics.DrawString(child.text.c_str(), -1, &font, pointF, &solidBrush);
                
                Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0), 2);
                graphics.DrawRectangle(&pen, child.x, child.y, child.width, child.height);
            }
        }

        // 结束
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:{
        auto win = EApplication.getEWindowByHWND(hwnd);
        EApplication.removeChild(win);
        delete win;
        auto num = EApplication.getChildrenLength();
        if (num == 0){
            EApplication.quit();
        }
        break;
    }

    default:
        break;
    }
    return EApplication.eventEmitter.emit(hwnd, message, wParam, lParam);
}