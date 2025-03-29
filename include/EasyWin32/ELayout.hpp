#pragma once
#include "string"
#include "vector"
class ELayout{
    private:
        std::vector<ELayout> children;
    public:

    template <typename T, typename... Args>
    ELayout& child(T layout, Args... args){
        (children.push_back(args), ...);
        return *this;
    }
};


ELayout& lRows();

ELayout& lText(std::wstring text);

ELayout& lButton(std::wstring text);