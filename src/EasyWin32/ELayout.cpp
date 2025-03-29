#include "ELayout.hpp"

ELayout& lRows(){
    auto layout = new ELayout();
    layout->type = LayoutType::ROWS;
    return *layout;
}

ELayout& lText(std::wstring text){
    auto layout = new ELayout();
    layout->type = LayoutType::TEXT;
    layout->setText(text);
    return *layout; 
}
ELayout& lButton(std::wstring text){
    auto layout = new ELayout();
    layout->type = LayoutType::BUTTON;
    layout->setText(text);
    return *layout; 
}