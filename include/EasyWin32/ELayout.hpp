#pragma once
#include "string"
#include "vector"

enum class LayoutType{
    ROWS, TEXT, BUTTON 
};

class ELayout{
    private:
        std::vector<ELayout> children;
        
    public:
    std::wstring text;
    LayoutType type;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;

    template <typename T, typename... Args>
    ELayout& child(T layout, Args... args){
        children.push_back(layout);
        (children.push_back(args), ...);
        switch (this->type)
        {
        case LayoutType::ROWS:{
            int size = this->children.size();
            int height = 50;
            for (int i = 0; i < size; i++){
                this->children[i].x = 20;
                this->children[i].y = 20 + i * height;
                this->children[i].width = 1160;
                this->children[i].height = height;
            }
            break;
        }
        default:
            break;
        }
        return *this;
    }
    ELayout& setText(std::wstring text){
        this->text = text;
        return *this; 
    }
    ELayout& margin(int32_t x, int32_t y){
        return *this;
    }
    std::vector<ELayout> getChildren(){
        return this->children; 
    }
};


ELayout& lRows();

ELayout& lText(std::wstring text);

ELayout& lButton(std::wstring text);