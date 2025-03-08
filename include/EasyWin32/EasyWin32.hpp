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
int Emain(int);
int Emain(int, char* []);
int Emain(HINSTANCE);
int Emain(HINSTANCE, HINSTANCE);
int Emain(HINSTANCE, HINSTANCE, PSTR);
int Emain(HINSTANCE, HINSTANCE, PSTR, int);

#define COUNT_ARGS_IMPL(_, _1, _2, _3, _4, _5, _6, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(_, ##__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)

/**
 * @brief WinMain的参数类型应为int, char* []
 *
 * 参数可以从最后一个开始忽略，但参数类型位置不能改变
 * 
 * 可以使用WinMain函数代替main函数
 * 
 * @param int 传入程序的参数个数
 * @param char* [] 传入程序的参数字符串数组
 */
#define main(...) MAIN_HELPER(__VA_ARGS__)

#define MAIN_HELPER(...)                                                                                      \
    _USER_MAIN_ENTRY(int argc, char* argv[], HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) \
    {                                                                                                         \
        if constexpr (COUNT_ARGS(__VA_ARGS__) == 0)                                                           \
        {                                                                                                     \
            return Emain();                                                                                   \
        }                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 1)                                                      \
        {                                                                                                     \
            return Emain(argc);                                                                         \
        }                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 2)                                                      \
        {                                                                                                     \
            return Emain(argc, argv);                                                                         \
        }                                                                                                     \
        return 0;                                                                                             \
    }                                                                                                         \
    int Emain(__VA_ARGS__)

/**
 * @brief WinMain的参数类型应为HINSTANCE, HINSTANCE, PSTR, int
 *
 * 参数可以从最后一个开始忽略，但参数类型位置不能改变
 * 
 * 可以使用main函数代替WinMain函数
 *
 * @param HINSTANCE 程序实例句柄
 * @param HINSTANCE 应用程序的上一个实例的句柄
 * @param PSTR 程序的命令行参数
 * @param int 窗口的显示方式
 */
#define WinMain(...) WINMAIN_HELPER(__VA_ARGS__)

#define WINMAIN_HELPER(...)                                                                                   \
    _USER_MAIN_ENTRY(int argc, char* argv[], HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) \
    {                                                                                                         \
        if constexpr (COUNT_ARGS(__VA_ARGS__) == 0)                                                           \
        {                                                                                                     \
            return Emain();                                                                                   \
        }                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 1)                                                      \
        {                                                                                                     \
            return Emain(hInst);                                                                              \
        }                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 2)                                                      \
        {                                                                                                     \
            return Emain(hInst, hInstPrev);                                                                   \
        }                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 3)                                                      \
        {                                                                                                     \
            return Emain(hInst, hInstPrev, cmdline);                                                          \
        }                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 4)                                                      \
        {                                                                                                     \
            return Emain(hInst, hInstPrev, cmdline, cmdshow);                                                 \
        }                                                                                                     \
        return 0;                                                                                             \
    }                                                                                                         \
    int Emain(__VA_ARGS__)
