#include "EWidget.hpp"

EWidget::EWidget(ENode *parent) : ENode(parent)
{
}

UINT32 EWidget::getWidth()
{
    return this->width;
}

std::wstring EWidget::getTitle()
{
    return this->title;
}