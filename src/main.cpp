#include "EasyWin32.hpp"
#include <string>

int main(int argc, char* argv[]){
    auto window = new EWindow();
    window->setTitle(L"Test")
    ->setSize(1500, 1000)
    ->setPosition(400,400)
    ->onDestroy(
        EApplication->quit();
    )
    ->onMouseLeftUp(
        EApplication->alert(std::to_wstring(EApplication->getDPI()));
    )
    ->show();

    return 0;
}