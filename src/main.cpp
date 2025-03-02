#include "EasyWin32.hpp"

int main(int argc, char* argv[]){
    auto window = new EWindow({.title="Test",.width = 1000, .height = 500});
    auto window2 = new EWindow({.title="Test2",.width = 1000,.height = 1000});

    window->on(WM_DESTROY, ECallback{
        EApplication->alert("exit...");
        EApplication->quit();
        return 0;
    });

    window->on(WM_PAINT, ECallback{
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
        std::string str = "Hello, World!";
        TextOut(hdc, 0, 0, str.c_str(), str.length());
        EndPaint(hwnd, &ps);
        return 0;
    });


    // 注册WM_PAINT回调后需要手动触发重绘才能立即生效
    window->rePaint();

    //测试窗口管理功能
    auto mywin2 = EApplication->getEWindowByTitle("Test2");
    mywin2->on(WM_DESTROY, ECallback{
        EApplication->alert("Test2");
        return 0;
    });

    return 0;
}