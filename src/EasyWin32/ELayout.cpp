#include "ELayout.hpp"

ELayout& lRows(){
    auto layout = new ELayout();
    return *layout;
}

ELayout& lText(std::wstring text){
    auto layout = new ELayout();
    return *layout; 
}
ELayout& lButton(std::wstring text){
    auto layout = new ELayout();
    return *layout; 
}