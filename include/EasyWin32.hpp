#pragma once
#include "types.hpp"
#include "EApplication.hpp"
#include "EWindow.hpp"
#include "EventEmitter.hpp"
#include "EWidget.hpp"

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