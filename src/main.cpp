#include "main.hpp"
#include "EasyWin32.hpp"
#include "var.hpp"


ELayout& lSayHello(std::wstring name){
    return lRows()
    .margin(20,20)
    .child(
        lText(L"Hello " + name),
        lButton(L"btn")
    );
}


int main(int argc, char* argv[]){
    auto window = EApplication.createWindow();

    window.setTitle(L"Test")
    .setSize(1500, 1000)
    .setPosition(400,400)
    .setLayout(lSayHello(L"World!"))
    .show();

    return 0;
}