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

    auto win1 = new EWindow();
    win1->show();

    auto win2 = new EWindow();
    win2->show();

    size_t num = EApplication->getChildrenLength();
    EApplication->alert(std::to_wstring(num));
    return 0;
}