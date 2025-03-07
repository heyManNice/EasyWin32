#include "EWindow.hpp"
#include "EApplication.hpp"

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include <winrt/Windows.UI.ViewManagement.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")

EWindow::EWindow() : EWidget(EApplication)
{
    this->width = 800;
    this->height = 600;
    this->initialize();
}

EWindow *EWindow::on(const UINT message, EventEmitterCallback callback)
{
    EApplication->eventEmitter.on(this->hwnd, message, callback);
    switch (message)
    {
    case WM_PAINT:
        // 如果注册WM_PAINT回调后视图立即生效
        this->rePaint();
        break;
    default:
        break;
    }
    return this;
}

void EWindow::rePaint()
{
    InvalidateRect(this->hwnd, NULL, TRUE);
}

HWND EWindow::getHWND()
{
    return this->hwnd;
}

EWindow *EWindow::setTitle(const std::wstring &title)
{
    this->title = title;
    SetWindowTextW(this->hwnd, title.c_str());
    return this;
}

EWindow *EWindow::setSize(const UINT32 width, const UINT32 height)
{
    this->width = width;
    this->height = height;
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    SetWindowPos(this->hwnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
    return this;
}

EWindow *EWindow::setPosition(const INT32 x, const INT32 y)
{
    this->x = x;
    this->y = y;
    SetWindowPos(this->hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    return this;
}

EWindow *EWindow::show()
{
    ShowWindow(this->hwnd, SW_SHOW);
    UpdateWindow(this->hwnd);
    return this;
}

void EWindow::initialize()
{
    // 创建窗口
    EApplication->updateDpiFromMonitor();
    this->hwnd = CreateWindowExW(NULL, L"EasyWin32ClassName", this->title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height, NULL, NULL, GetModuleHandle(NULL), NULL);
    if (this->hwnd == NULL)
    {
        int err = GetLastError();
        wchar_t errorMsg[256];
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorMsg, 256, NULL);
        EApplication->alert(errorMsg, L"错误代码: " + std::to_wstring(err));
        exit(1);
    }
    this->updateThemeMode();
}

void EWindow::updateThemeMode()
{
    BOOL isDark = FALSE;
    switch (this->theme)
    {
    case ThemeMode::Light:
        isDark = FALSE;
        break;
    case ThemeMode::Dark:
        isDark = TRUE;
        break;
    default:
    {
        auto settings = winrt::Windows::UI::ViewManagement::UISettings();
        auto clr = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
        isDark = ((5 * clr.G) + (2 * clr.R) + clr.B) > (8 * 128);
        break;
    }
    }
    // 20 is DWMWA_USE_IMMERSIVE_DARK_MODE
    DwmSetWindowAttribute(this->hwnd, 20, &isDark, sizeof(isDark));
    UpdateWindow(this->hwnd);
}

void EWindow::setThemeMode(ThemeMode theme)
{
    this->theme = theme;
    this->updateThemeMode();
}

LRESULT CALLBACK EWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    // 系统主题改变时更新窗口主题色
    case WM_SETTINGCHANGE:
    {
        auto window = EApplication->getEWindowByHWND(hwnd);
        if (window != nullptr)
        {
            window->updateThemeMode();
        }
        // 更新dpi
        EApplication->updateDpiFromMonitor();
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
        int dpi = EApplication->getDPI();
        double scale = (double)dpi / 96;
        graphics.ScaleTransform(scale, scale);

        // 绘图
        Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0));
        pen.SetWidth(10);
        graphics.DrawRectangle(&pen, 10, 10, 200, 100);
        graphics.DrawEllipse(&pen, 500, 500, 100, 100);
        Gdiplus::FontFamily fontFamily(L"HarmonyOS Sans sC");
        Gdiplus::Font font(&fontFamily, 100, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 0, 0, 0));
        Gdiplus::PointF pointF(100, 300);
        graphics.DrawString(L"Hello, GDI+!鸿蒙系统", -1, &font, pointF, &solidBrush);

        // 结束
        EndPaint(hwnd, &ps);
        break;
    }

    default:
        break;
    }
    return EApplication->eventEmitter.emit(hwnd, message, wParam, lParam);
}
