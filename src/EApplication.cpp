#include "EApplication.hpp"

#include <windows.h>
#include <string>
#include <map>

EApplicationSingleton* EApplicationSingleton::getInstance(){
    if (instance == nullptr) {
        instance = new EApplicationSingleton();
    }
    return instance;
}

int EApplicationSingleton::alert(const std::string& message, const std::string& caption = "Alert", UINT type = MB_OK) {
    return MessageBox(NULL, message.c_str(),caption.c_str(), type);
}

int EApplicationSingleton::quit() {
    PostQuitMessage(0);
    return 0;
}
int EApplicationSingleton::getDPI() {
    return this->dpi; 
}

void EApplicationSingleton::_setIsWinMainEntry(BOOL value) {
    this->isWinMainEntry = value; 
}

BOOL EApplicationSingleton::getIsWinMainEntry() {
    return this->isWinMainEntry; 
}