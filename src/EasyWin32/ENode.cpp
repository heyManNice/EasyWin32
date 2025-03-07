#include "ENode.hpp"

ENode::ENode(ENode* parent){
    if(parent != nullptr){
        this->parent = parent;
        this->parent->addChild(this);
    }
    
}
ENode::~ENode(){
    this->parent->removeChild(this);
    for (auto child : this->children){
        delete child;
    }
}

ENode* ENode::addChild(ENode* child){
    this->children.push_back(child);
    return this;
}

ENode* ENode::removeChild(ENode* child){
    for (auto it = this->children.begin(); it != this->children.end(); it++){
        if (*it == child){
            this->children.erase(it);
            return this;
        }
    }
    return this;
}

ENode* ENode::getParent() const{
    return this->parent;
}

size_t ENode::getChildrenLength() const{
    return this->children.size();  
}