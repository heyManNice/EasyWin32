#include "main.hpp"
#include "EasyWin32.hpp"

int main(int argc, char* argv[]){
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