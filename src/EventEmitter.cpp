#include "EventEmitter.hpp"


LRESULT EventEmitter::emit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if( HwndMap.find(hwnd) == HwndMap.end() ||
        HwndMap[hwnd].find(message) == HwndMap[hwnd].end())
    {
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    LRESULT callbackResult = 0;
    LRESULT callbackCount = 0;
    for(auto callback : HwndMap[hwnd][message]) {
        callbackCount++;
        callbackResult += callback(hwnd, message, wParam, lParam);
    }

    //返回值用于告诉系统是否已处理信息
    if(message == WM_ERASEBKGND || message == WM_SETCURSOR){
        //这两个事件比较特殊，需要返回1表示已处理
        if(callbackResult && callbackCount) return 1;
    }else{
        //其他事件需要返回0表示已处理
        if(!callbackResult && callbackCount) return 0;
    }
    //如果用户的回调函数没有通知系统已处理信息，则返回系统默认处理结果
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