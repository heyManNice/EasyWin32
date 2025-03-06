#pragma once
#include "types.hpp"

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
        LRESULT emit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    
        /**
         * @brief 事件订阅函数
         * 
         * @param hwnd 该事件所属的窗口句柄
         * @param message 该事件的消息
         * @param callback 该事件触发时的回调函数
         */
        void on(HWND hwnd, UINT message,EventEmitterCallback callback);
    };
    