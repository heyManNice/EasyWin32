#pragma once
#include "types.hpp"

/**
 * @brief main函数，给用户一个入口，用户可以在这里编写自己的代码
 * 真实的入口是WinMain函数，已经在EasyWin32.cpp中实现。
 * 用户无需编写WinMain函数，只需要编写main函数即可。
 * 
 * @param argc 传入程序的参数个数
 * @param argv 传入程序的参数字符串数组
 * @return int 程序运行结束后的返回值，由用户给定
 */
int main(int argc, char* argv[]);
int Emain(int argc, char* argv[]);

#define main(...) Emain(__VA_ARGS__)


/**
 * @brief 基础控件对象类
 * 
 */
class EObject{
public:
    std::string title = "EasyWin32";
    INT32 x = 0;
    INT32 y = 0;
    UINT32 width = 100;
    UINT32 height = 100;
    BOOL visible = TRUE;
    BOOL enabled = TRUE;
};





/**
 * @brief 控件类
 * 
 */
class EWidget : public EObject {
private:
    friend class EWindow;
    ThemeMode theme = ThemeMode::Auto;
    EWidget* parent;
public:
    EWidget(EWidget* parent) {
        this->parent = parent;
    }
    EWidget(const EObject &object): EObject(object) {
        this->parent = nullptr;
    }
    EWidget() {
        this->parent = nullptr; 
    }
    UINT32 getWidth() {
        return this->width;
    }
    ~EWidget() {}
};



/**
 * @brief 窗口类
 * 
 */
class EWindow : public EWidget {
private:
    friend class EApplicationSingleton;

    HWND hwnd;
    WNDCLASSEX wcex;
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void initialize();
    /**
     * @brief 更新窗口主题色
     * 
     */
    void updateThemeMode();
public:
    EWindow() {
        this->width = 800;
        this->height = 600;
        this->initialize();
        EApplication->addWindow(this); 
    }
    ~EWindow() {
        EApplication->removeWindow(this); 
    }
    /**
     * @brief 注册事件
     * 
     * @param message 事件类型
     * @param callback 回调函数
     */
    EWindow* on(const UINT message, EventEmitterCallback callback) {
        EApplication->eventEmitter.on(this->hwnd,message,callback);
        switch (message)
        {
        case WM_PAINT:
            //如果注册WM_PAINT回调后视图立即生效
            this->rePaint();
            break;
        default:
            break;
        }
        return this;
    }
    /**
     * @brief 重绘窗口
     *
     */
    void rePaint() {
        InvalidateRect(this->hwnd, NULL, TRUE); 
    }
    /**
     * @brief 获取窗口句柄
     * 
     */
    HWND getHWND() {
        return this->hwnd; 
    }
    /**
     * @brief 设置窗口主题色
     * 
     * 自动更新到窗口
     * @param theme 
     */
    void setThemeMode(ThemeMode theme);

    /**
     * @brief 设置标题
     * 
     */
    EWindow* setTitle(const std::string& title) {
        this->title = title;
        SetWindowText(this->hwnd, title.c_str());
        return this;
    }

    /**
     * @brief 设置宽高
     * 
     */
    EWindow* setSize(const UINT32 width, const UINT32 height) {
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

    /**
     * @brief 设置位置
     *
     */
    EWindow* setPosition(const INT32 x, const INT32 y) {
        this->x = x;
        this->y = y;
        SetWindowPos(this->hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        return this;
    }

    /**
     * @brief 显示窗口
     * 
     */
    EWindow* show() {
        ShowWindow(this->hwnd, SW_SHOW);
        UpdateWindow(this->hwnd);
        return this; 
    }
};
