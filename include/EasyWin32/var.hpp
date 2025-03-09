#pragma once
#include <string>

/**
 * @brief 在c++里实现弱类型
 * 
 * 基于隐式转换和重载运算符实现
 * 计划支持的类型:
 * string
 * number
 * array
 * object
 * underfined
 * boolean
 */

enum class VarType
{
    Int,
    stdString,
};



class var
{
private:
    int int_value;
    std::string std_string_value;
    VarType type;
public:
    //初始化为int
    var(int value)
    {   
        this->int_value = value;
        type = VarType::Int;
    }
    //隐式转换出int
    operator int() const
    {
        switch (this->type)
        {
        case VarType::Int:{
            return this->int_value;
        }

        case VarType::stdString:{
            return std::stoi(this->std_string_value); 
        }
        default:
            return 0;
        }
    }

    //重载+
    var operator+(const char* other) const
    {
        switch (this->type)
        {
        case VarType::stdString:{
            return this->std_string_value + std::string(other); 
        }
        default:
            return 0;
        }
    }
    var operator+(const var& other) const{
        switch (other.type){
        case VarType::Int:{
            return this->std_string_value + std::to_string(other.int_value); 
        }
        case VarType::stdString:{
            return this->std_string_value + other.std_string_value;
        }
        default:
            return 0;
        }
    }



    //初始化为std::string
    var(std::string value)
    {
        this->std_string_value = value;
        type = VarType::stdString;
    }
    var(const char* value)
    {
        this->std_string_value = value;
        type = VarType::stdString;
    }
    //隐式转换出std::string
    operator std::string() const{
        switch (this->type)
        {
        case VarType::Int:{
            return std::to_string(this->int_value); 
        }
        case VarType::stdString:{
            return this->std_string_value; 
        }
        default:
            return "";
        }
    }
};