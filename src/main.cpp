#include "main.hpp"
#include "EasyWin32.hpp"
#include "var.hpp"

int main(int argc, char* argv[]){

    var str1 = "hello";
    var str2 = str1 + " world!";
    var num1 = 300;
    var str3 = str2 + num1;

    var num2 = "200";

    //验证
    std::string str = str3;
    int num = num2;

    auto window = new EWindow();
    window->setTitle(L"Test")
    ->setSize(1500, 1000)
    ->setPosition(400,400)
    ->onDestroy(
        EApplication->quit();
    )
    ->onMouseLeftUp(
        size_t num = EApplication->getChildrenLength();
        EApplication->alert(std::to_wstring(num));
    )
    ->show();

    return 0;
}