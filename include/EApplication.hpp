#pragma once
#include "types.hpp"
#include "EventEmitter.hpp"
#include "ENode.hpp"

/**
 * @brief 应用程序类，用于创建应用程序
 * 这是一个单例类，在一个项目中只能创建一个实例。
 */
class EApplicationSingleton : public ENode
{
private:
    friend class EWindow;

    /**
     * @brief 单例对象
     *
     */
    static EApplicationSingleton *instance;
    EApplicationSingleton() {}
    ~EApplicationSingleton() {}
    EApplicationSingleton(const EApplicationSingleton &) {}
    EApplicationSingleton &operator=(const EApplicationSingleton &) {}

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
    BOOL addWindow(EWindow *window);

    /**
     * @brief 删除窗口
     *
     * @param hwnd
     * @return BOOL
     */
    BOOL removeWindow(EWindow *window);

    /**
     * @brief 入口函数是winMain的标记
     *
     */
    BOOL isWinMainEntry = TRUE;

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
    static EApplicationSingleton *getInstance();
    /**
     * @brief 发出警告框
     *
     * @param message 提示信息
     * @param ?caption 标题
     * @param ?type 类型
     * @return int
     */
    int alert(const std::wstring &message, const std::wstring &caption = L"Alert", UINT type = MB_OK);

    /**
     * @brief 退出应用程序
     *
     * @return int 0
     */
    int quit();

    /**
     * @brief 通过标题名称获取窗口实例
     *
     * @param title 标题名称
     * @return EWindow* 窗口实例,找不到则返回nullptr
     */
    EWindow *getEWindowByTitle(const std::wstring &title);

    /**
     * @brief 通过句柄获取窗口实例
     *
     * @param hwnd 窗口句柄
     * @return EWindow* 窗口实例,找不到则返回nullptr
     */
    EWindow *getEWindowByHWND(HWND hwnd);

    /**
     * @brief 获取当前应用程序的dpi
     *
     */
    int getDPI();

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
    void _setIsWinMainEntry(BOOL value);

    /**
     * @brief 获取当前程序的入口函数是否是winMain
     *
     * @return BOOL
     */
    BOOL getIsWinMainEntry();
};