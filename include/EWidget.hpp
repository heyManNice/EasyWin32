#pragma once
#include "types.hpp"

/**
 * @brief 基础控件对象类
 *
 */
class EObject
{
public:
    std::wstring title = L"EasyWin32";
    INT32 x = 0;
    INT32 y = 0;
    UINT32 width = 100;
    UINT32 height = 100;
    BOOL visible = TRUE;
    BOOL enabled = TRUE;
};

/**
 * @brief 控件类
 *
 */
class EWidget : public EObject
{
private:
    friend class EWindow;
    ThemeMode theme = ThemeMode::Auto;
    EWidget *parent;

public:
    EWidget(EWidget *parent)
    {
        this->parent = parent;
    }
    EWidget(const EObject &object) : EObject(object)
    {
        this->parent = nullptr;
    }
    EWidget()
    {
        this->parent = nullptr;
    }
    UINT32 getWidth()
    {
        return this->width;
    }
    ~EWidget() {}
};
