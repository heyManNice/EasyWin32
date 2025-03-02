#include "EasyWin32.hpp"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    int ret = main(__argc, __argv);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return ret;
}

EApplicationSingleton* EApplicationSingleton::instance = nullptr;

LRESULT CALLBACK EWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
    this->hwnd = CreateWindowEx(NULL,this->title.c_str(), this->title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height, NULL, NULL, this->wcex.hInstance, NULL);
    ShowWindow(this->hwnd, SW_SHOW);
    UpdateWindow(this->hwnd);
}