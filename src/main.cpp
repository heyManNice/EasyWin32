#include "main.hpp"
#include "EasyWin32.hpp"
#include "var.hpp"


ELayout& lSayHello(){
    return lRows()
    .margin(20,20)
    .child(
        lText(L"Hello World!"),
        lButton(L"btn")
    );
}


int main(int argc, char* argv[]){
    auto window = EApplication.createWindow();

    window.setTitle(L"Test")
    .setSize(1500, 1000)
    .setPosition(400,400)
    .setLayout(lSayHello())
    .onDestroy(
        EApplication.quit();
    )
    .onMouseLeftUp(
        size_t num = EApplication.getChildrenLength();
        EApplication.alert(std::to_wstring(num));
    )
    .show();

    return 0;
}