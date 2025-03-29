#pragma once

#include "types.hpp"
#include "EWidget.hpp"
#include "ELayout.hpp"

/**
 * @brief 窗口类
 * 
 */
class EWindow : public EWidget {
    private:
        friend class EApplicationSingleton;
    
        HWND hwnd;

        ELayout& layout = lText(L"none");
        
        void initialize();
        /**
         * @brief 更新窗口主题色
         * 
         */
        void updateThemeMode();
    public:
        EWindow();
        /**
         * @brief 注册事件
         * 
         * @param message 事件类型
         * @param callback 回调函数
         */
        EWindow& on(const UINT message, EventEmitterCallback callback);
        /**
         * @brief 重绘窗口
         *
         */
        void rePaint();
        /**
         * @brief 获取窗口句柄
         * 
         */
        HWND getHWND();

        /**
         * @brief 获取布局
         * 
         */
        ELayout& getLayout();
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
        EWindow& setTitle(const std::wstring& title);
    
        /**
         * @brief 设置宽高
         * 
         */
        EWindow& setSize(const UINT32 width, const UINT32 height);
    
        /**
         * @brief 设置位置
         *
         */
        EWindow& setPosition(const INT32 x, const INT32 y);
        /**
         * @brief 设置布局
         * 
         */
        EWindow& setLayout(ELayout& layout);
    
        /**
         * @brief 显示窗口
         * 
         */
        EWindow& show();
};
    