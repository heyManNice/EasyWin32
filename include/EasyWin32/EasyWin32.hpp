#pragma once
#include "types.hpp"
#include "EApplication.hpp"
#include "EWindow.hpp"
#include "EventEmitter.hpp"
#include "EWidget.hpp"
#include "ELayout.hpp"

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

#define COUNT_ARGS_IMPL(_, _1, _2, _3, _4, _5, _6, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(_, ##__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)

/**
 * @brief main的参数类型应为int, char* []
 *
 * 参数可以从最后一个开始忽略，但参数类型位置不能改变
 *
 * 可以使用WinMain函数代替main函数
 *
 * @param int 传入程序的参数个数
 * @param char* [] 传入程序的参数字符串数组
 */
#define main(...) MAIN_HELPER(__VA_ARGS__)

/**
 * @brief 检查main函数的参数类型是否匹配并且返回对应的函数指针
 *
 * 使用函数指针是为了防止编译编译器在还没有计算移除常量表达式分支
 * 的时候进行语法检查导致的找不到函数定义错误
 *
 * @param when 是否检查参数类型
 * @param ... 函数的正确参数类型
 * @example
 * CHECK_MAIN_PARAMS_AND_CALL(num==2, )(int,int);
 */
#define CHECK_MAIN_PARAMS_AND_CALL(when, ...)                                                                                                 \
    static_assert(!(when) || std::is_same_v<decltype(&Emain), int (*)(__VA_ARGS__)>, "main函数的类型不匹配！参数类型应为(" #__VA_ARGS__ ")"); \
    int (*func)(__VA_ARGS__) = (int (*)(__VA_ARGS__))Emain;                                                                                   \
    return func

#define CHECK_WINMAIN_PARAMS_AND_CALL(when, ...)                                                                                                 \
    static_assert(!(when) || std::is_same_v<decltype(&Emain), int (*)(__VA_ARGS__)>, "WinMain函数的类型不匹配！参数类型应为(" #__VA_ARGS__ ")"); \
    int (*func)(__VA_ARGS__) = (int (*)(__VA_ARGS__))Emain;                                                                                      \
    return func

#define MAIN_HELPER(...)                                                                                          \
    Emain(__VA_ARGS__);                                                                                           \
    int _USER_MAIN_ENTRY(int argc, char *argv[], HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) \
    {                                                                                                             \
        if constexpr (COUNT_ARGS(__VA_ARGS__) == 0)                                                               \
        {                                                                                                         \
            CHECK_MAIN_PARAMS_AND_CALL(COUNT_ARGS(__VA_ARGS__) == 0, )();                                         \
        }                                                                                                         \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 1)                                                          \
        {                                                                                                         \
            CHECK_MAIN_PARAMS_AND_CALL(COUNT_ARGS(__VA_ARGS__) == 1, int)(argc);                                  \
        }                                                                                                         \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 2)                                                          \
        {                                                                                                         \
            CHECK_MAIN_PARAMS_AND_CALL(COUNT_ARGS(__VA_ARGS__) == 2, int, char *[])(argc, argv);                  \
        }                                                                                                         \
        else                                                                                                      \
        {                                                                                                         \
            static_assert(!(COUNT_ARGS(__VA_ARGS__) > 2), "main函数的参数数量必须是0~2个");                       \
        }                                                                                                         \
        return 0;                                                                                                 \
    }                                                                                                             \
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

#define WINMAIN_HELPER(...)                                                                                                                   \
    Emain(__VA_ARGS__);                                                                                                                       \
    int _USER_MAIN_ENTRY(int argc, char *argv[], HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)                                 \
    {                                                                                                                                         \
        if constexpr (COUNT_ARGS(__VA_ARGS__) == 0)                                                                                           \
        {                                                                                                                                     \
            CHECK_WINMAIN_PARAMS_AND_CALL(COUNT_ARGS(__VA_ARGS__) == 0, )();                                                                  \
        }                                                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 1)                                                                                      \
        {                                                                                                                                     \
            CHECK_WINMAIN_PARAMS_AND_CALL(COUNT_ARGS(__VA_ARGS__) == 1, HINSTANCE)(hInst);                                                    \
        }                                                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 2)                                                                                      \
        {                                                                                                                                     \
            CHECK_WINMAIN_PARAMS_AND_CALL(COUNT_ARGS(__VA_ARGS__) == 2, HINSTANCE, HINSTANCE)(hInst, hInstPrev);                              \
        }                                                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 3)                                                                                      \
        {                                                                                                                                     \
            CHECK_WINMAIN_PARAMS_AND_CALL(COUNT_ARGS(__VA_ARGS__) == 3, HINSTANCE, HINSTANCE, PSTR)(hInst, hInstPrev, cmdline);               \
        }                                                                                                                                     \
        else if constexpr (COUNT_ARGS(__VA_ARGS__) == 4)                                                                                      \
        {                                                                                                                                     \
            CHECK_WINMAIN_PARAMS_AND_CALL(COUNT_ARGS(__VA_ARGS__) == 4, HINSTANCE, HINSTANCE, PSTR, int)(hInst, hInstPrev, cmdline, cmdshow); \
        }                                                                                                                                     \
        else                                                                                                                                  \
        {                                                                                                                                     \
            static_assert(!(COUNT_ARGS(__VA_ARGS__) > 4), "main函数的参数数量必须是0~4个");                                                   \
        }                                                                                                                                     \
        return 0;                                                                                                                             \
    }                                                                                                                                         \
    int Emain(__VA_ARGS__)
