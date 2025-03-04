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
 * @brief 发布订阅模式类
 * 
 */
class EventEmitter{
private:
    /**
     * @brief 事件回调函数映射表
     * 
     * HWND -> Event -> CallbackList -> Callback
     * Map首先由HWND索引，然后由Event索引，最后由CallbackList索引，最后是Callback的Vector
     */
    EventEmitterHWNDMap HwndMap;
public:
    EventEmitter () {}
    ~EventEmitter () {}

    /**
     * @brief 事件发布函数
     *
     * @param hwnd 事件所属的窗口句柄
     * @param message 事件的类型
     * @param wParam 事件的wParam
     * @param lParam 事件的lParam
     * @return LRESULT 事件的返回值
     */
    LRESULT emit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if(HwndMap.find(hwnd) != HwndMap.end()) {
            if(HwndMap[hwnd].find(message) != HwndMap[hwnd].end()) {
                for(auto callback : HwndMap[hwnd][message]) {
                    callback(hwnd, message, wParam, lParam);
                } 
            }
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    /**
     * @brief 事件订阅函数
     * 
     * @param hwnd 该事件所属的窗口句柄
     * @param message 该事件的消息
     * @param callback 该事件触发时的回调函数
     */
    void on(HWND hwnd, UINT message,EventEmitterCallback callback) {
        if(HwndMap.find(hwnd)== HwndMap.end()) {
            HwndMap[hwnd] = EventEmitterEventMap();
        }
        if(HwndMap[hwnd].find(message)== HwndMap[hwnd].end()) {
            HwndMap[hwnd][message] = EventEmitterCallbackList();
        }
        HwndMap[hwnd][message].push_back(callback);
    }
};




/**
 * @brief 应用程序类，用于创建应用程序
 * 这是一个单例类，在一个项目中只能创建一个实例。
 */
class EApplicationSingleton  {
private:
    friend class EWindow;

    /**
     * @brief 单例对象
     * 
     */
    static EApplicationSingleton* instance;
    EApplicationSingleton(){}
    ~EApplicationSingleton(){}
    EApplicationSingleton(const EApplicationSingleton&){}
    EApplicationSingleton& operator=(const EApplicationSingleton&){}

    /**
     * @brief 窗口映射表，使用句柄作为索引
     * 
     */
    EWindowMap windowMap;

    /**
     * @brief 应用程序的dpi，默认为96
     * 
     */
    int dpi = 96;

    /**
     * @brief 添加窗口
     * 
     * @param hwnd
     * @param window 
     * @return BOOL 
     */
    BOOL addWindow(EWindow* window);

    /**
     * @brief 删除窗口
     *
     * @param hwnd
     * @return BOOL
     */
    BOOL removeWindow(EWindow* window);

    /**
     * @brief 入口函数是winMain的标记
     * 
     */
    BOOL isWinMainEntry = FALSE;
public:
    /**
     * @brief 事件发布订阅器
     * 
     */
    EventEmitter eventEmitter;
    /**
     * @brief 获取单例对象
     * 
     * @return EApplicationSingleton* 
     */
    static EApplicationSingleton* getInstance() {
        if (instance == nullptr) {
            instance = new EApplicationSingleton();
        }
        return instance;
    }
    /**
     * @brief 发出警告框
     * 
     * @param message 提示信息
     * @param ?caption 标题
     * @param ?type 类型
     * @return int 
     */
    int alert(const std::string& message, const std::string& caption = "Alert", UINT type = MB_OK) {
        return MessageBox(NULL, message.c_str(),caption.c_str(), type);
    }
    
    /**
     * @brief 退出应用程序
     * 
     * @return int 0
     */
    int quit() {
        PostQuitMessage(0);
        return 0;
    }

    /**
     * @brief 通过标题名称获取窗口实例
     * 
     * @param title 标题名称
     * @return EWindow* 窗口实例,找不到则返回nullptr
     */
    EWindow* getEWindowByTitle(const std::string& title);

    /**
     * @brief 通过句柄获取窗口实例
     *
     * @param hwnd 窗口句柄
     * @return EWindow* 窗口实例,找不到则返回nullptr
     */
    EWindow* getEWindowByHWND(HWND hwnd);

    /**
     * @brief 获取当前应用程序的dpi
     * 
     */
    int getDPI() {
        return this->dpi; 
    }

    /**
     * @brief 从显示器获取dpi
     * 
     */
    void updateDpiFromMonitor();

    /**
     * @brief 设置入口函数是winMain的标记
     * 
     * 在winMain中调用
     */
    void _setIsWinMainEntry() {
        this->isWinMainEntry = TRUE; 
    }

    /**
     * @brief 获取当前程序的入口函数是否是winMain
     * 
     * @return BOOL 
     */
    BOOL getIsWinMainEntry() {
        return this->isWinMainEntry; 
    }
};






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
