#pragma once
#include "types.hpp"
#include "EApplication.hpp"
#include "EWindow.hpp"
#include "EventEmitter.hpp"
#include "EWidget.hpp"

/**
 * @brief Emain函数，给用户一个入口，用户可以在这里编写自己的代码
 * 真实的入口是WinMain函数或者main函数，已经在EasyWin32.cpp中实现。
 * 用户无需编写WinMain函数或者main，只需要编写Emain函数即可。
 * 如果用户使用main函数，EasyWin32会自动将其转换为Emain函数。
 *
 * @param argc 传入程序的参数个数
 * @param argv 传入程序的参数字符串数组
 * @return int 程序运行结束后的返回值，由用户给定
 */

int Emain();
int Emain(int, char *[]);

#define COUNT_ARGS_IMPL(_, _1, _2, _3, _4, _5, _6, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(_, ##__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)

/**
 * @brief 用户程序入口
 * 参数类型可以是空的，也可以是int, char* []
 *
 */
#define main(...) MAIN_HELPER(__VA_ARGS__)

#define MAIN_HELPER(...)                                 \
    _USER_MAIN_ENTRY(int argc, char *argv[])             \
    {                                                    \
        if constexpr (COUNT_ARGS(__VA_ARGS__) == 0)      \
        {                                                \
            return Emain();                              \
        }                                                \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 2) \
        {                                                \
            return Emain(argc, argv);                    \
        }                                                \
        return 0;                                        \
    }                                                    \
    int Emain(__VA_ARGS__)

/**
 * @brief 用户应该使用main(...)作为程序入口而不是winMain(...)
 *
 */
#define WinMain(...)                                                 \
    ENTRYERROR()                                                     \
    {                                                                \
        static_assert(false, "你应该使用main(...)代替winMain(...)"); \
    }                                                                \
    int main(int argc, char *argv[])
