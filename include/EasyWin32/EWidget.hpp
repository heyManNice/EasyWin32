#pragma once
#include "types.hpp"
#include "ENode.hpp"

/**
 * @brief 控件类
 *
 */
class EWidget : public ENode
{
private:
    friend class EWindow;
    ThemeMode theme = ThemeMode::Auto;
    std::wstring title = L"EasyWin32";
    INT32 x = 0;
    INT32 y = 0;
    UINT32 width = 100;
    UINT32 height = 100;
    BOOL visible = TRUE;
    BOOL enabled = TRUE;

public:
    EWidget(ENode *parent);
    UINT32 getWidth();
    std::wstring getTitle();
};
