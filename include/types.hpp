#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <functional>
#include <map>

class EWindow;


/**
 * @brief 发布订阅模式中回调函数的类型
 * 
 */
typedef std::function<LRESULT(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)> EventEmitterCallback;

/**
 * @brief 发布订阅模式中回调函数列表的类型
 *
 */
typedef std::vector<EventEmitterCallback> EventEmitterCallbackList;

/**
 * @brief 发布订阅模式中事件映射表的类型
 *
 */
typedef std::map<UINT, EventEmitterCallbackList>  EventEmitterEventMap;

/**
 * @brief 发布订阅模式中窗口句柄映射表的类型
 *
 */
typedef std::map<HWND,EventEmitterEventMap> EventEmitterHWNDMap;

/**
 * @brief 订阅回调函数参数宏
 *
 */
#define ECallback [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT



/**
 * @brief 获取EApplication单例对象
 * 
 * @return EApplicationSingleton* 单例对象
 */
#define EApplication EApplicationSingleton::getInstance()


/**
 * @brief 窗口映射表，使用句柄作为索引
 * 
 */
typedef std::map<HWND, EWindow*> EWindowMap;


/**
 * @brief 窗口主题色枚举
 * 
 */
enum class ThemeMode {
    Light,// 浅色主题
    Dark,// 深色主题
    Auto// 伴随系统自动主题
};


/**
 * @brief 事件注册的便捷宏
 * 
 */
#define onEvent(event,callback) on(event,ECallback{callback;return 0;})
#define onMouseLeftUp(callback) onEvent(WM_LBUTTONUP,callback)
#define onDestroy(callback) onEvent(WM_DESTROY,callback)