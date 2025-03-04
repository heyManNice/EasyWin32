#include "EasyWin32.hpp"
#include <winrt/Windows.UI.ViewManagement.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")

#include <iostream>


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    EApplication->_setIsWinMainEntry();
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    int ret = Emain(__argc, __argv);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return ret;
}

#ifdef main
#undef main
int main(int argc, char* argv[]){
    if(!EApplication->getIsWinMainEntry()) {
        SetConsoleOutputCP(CP_UTF8);
        std::cout << "\033[1;33m";//黄色
        std::cout << "[WARN] 当前以控制台模式运行，请为编译器添加WIN32标记以启用GUI模式(无控制台)";
        std::cout << "\033[0m";
        return WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWDEFAULT);
    }
    return Emain(argc, argv);
}
#endif
#define main(...) Emain(__VA_ARGS__)

EApplicationSingleton* EApplicationSingleton::instance = nullptr;

LRESULT CALLBACK EWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    //系统主题改变时更新窗口主题色
    case WM_SETTINGCHANGE:{
        auto window = EApplication->getEWindowByHWND(hwnd);
        if(window != nullptr) {
            window->updateThemeMode();
        }
        //更新dpi
        EApplication->updateDpiFromMonitor();
        break;
        }

    case WM_PAINT:{
        //初始化
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        int dpi = EApplication->getDPI();
        double scale = (double)dpi / 96;
        graphics.ScaleTransform(scale, scale);
        
        //绘图
        Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0));
        pen.SetWidth(10);
        graphics.DrawRectangle(&pen, 10, 10, 200, 100);
        graphics.DrawEllipse(&pen, 500, 500, 100, 100);
        Gdiplus::FontFamily fontFamily(L"HarmonyOS Sans sC");
        Gdiplus::Font font(&fontFamily, 100, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 0, 0, 0));
        Gdiplus::PointF pointF(100, 300);
        graphics.DrawString(L"Hello, GDI+!鸿蒙系统", -1, &font, pointF, &solidBrush);


        //结束
        EndPaint(hwnd, &ps);
        break;
        }

    default:
        break;
    }
    return EApplication->eventEmitter.emit(hwnd, message, wParam, lParam);
}



EWindow* EApplicationSingleton::getEWindowByTitle(const std::string& title) {
    for(auto it = this->windowMap.begin(); it != this->windowMap.end(); it++) {
        if(it->second->title == title) {
            return it->second;
        } 
    }
    return nullptr;
}

EWindow* EApplicationSingleton::getEWindowByHWND(HWND hwnd){
    if(auto it = this->windowMap.find(hwnd); it!= this->windowMap.end()) {
        return it->second; 
    }
    return nullptr;
}


BOOL EApplicationSingleton::addWindow(EWindow* window) {
    if(this->windowMap.find(window->hwnd) != this->windowMap.end()) {
        return FALSE;
    }
    this->windowMap[window->hwnd] = window;
    return TRUE;
}

BOOL EApplicationSingleton::removeWindow(EWindow* window) {
    if(this->windowMap.find(window->hwnd) == this->windowMap.end()) {
        return FALSE;
    }
    this->windowMap.erase(window->hwnd);
    return TRUE;
}

void EApplicationSingleton::updateDpiFromMonitor(){
    HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTONEAREST);
    UINT dpiX, dpiY;
    GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    this->dpi = dpiX;
}

void EWindow::initialize() {
    // 注册窗口类
    this->wcex.cbSize = sizeof(WNDCLASSEX);
    this->wcex.style = CS_HREDRAW | CS_VREDRAW;
    this->wcex.lpfnWndProc = &EWindow::WndProc;
    this->wcex.cbClsExtra = 0;
    this->wcex.cbWndExtra = 0;
    this->wcex.hInstance = GetModuleHandle(NULL);
    this->wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    this->wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    this->wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    this->wcex.lpszMenuName = NULL;
    this->wcex.lpszClassName = this->title.c_str();
    this->wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&this->wcex);

    // 创建窗口
    EApplication->updateDpiFromMonitor();
    this->hwnd = CreateWindowEx(NULL,this->title.c_str(), this->title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height, NULL, NULL, this->wcex.hInstance, NULL);
    this->updateThemeMode();
}

void EWindow::updateThemeMode() {
    BOOL isDark = FALSE;
    switch (this->theme)
    {
    case ThemeMode::Light:
        isDark = FALSE;
        break;
    case ThemeMode::Dark:
        isDark = TRUE;
        break;
    default:{
        auto settings = winrt::Windows::UI::ViewManagement::UISettings();
        auto clr = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
        isDark = ((5 * clr.G) + (2 * clr.R) + clr.B) > (8 * 128);
        break;
        }
    }
    //20 is DWMWA_USE_IMMERSIVE_DARK_MODE
    DwmSetWindowAttribute(this->hwnd, 20, &isDark, sizeof(isDark));
    UpdateWindow(this->hwnd);
}

void EWindow::setThemeMode(ThemeMode theme) {
    this->theme = theme;
    this->updateThemeMode(); 
}