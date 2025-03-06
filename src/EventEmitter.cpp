#include "EventEmitter.hpp"


LRESULT EventEmitter::emit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(HwndMap.find(hwnd) != HwndMap.end()) {
        if(HwndMap[hwnd].find(message) != HwndMap[hwnd].end()) {
            for(auto callback : HwndMap[hwnd][message]) {
                callback(hwnd, message, wParam, lParam);
            } 
        }
    }
    return DefWindowProcW(hwnd, message, wParam, lParam);
}

void EventEmitter::on(HWND hwnd, UINT message,EventEmitterCallback callback) {
    if(HwndMap.find(hwnd)== HwndMap.end()) {
        HwndMap[hwnd] = EventEmitterEventMap();
    }
    if(HwndMap[hwnd].find(message)== HwndMap[hwnd].end()) {
        HwndMap[hwnd][message] = EventEmitterCallbackList();
    }
    HwndMap[hwnd][message].push_back(callback);
}