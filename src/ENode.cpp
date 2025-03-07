#include "ENode.hpp"

ENode::ENode(ENode* parent){
    if(parent != nullptr){
        this->parent = parent;
        this->parent->addChild(this);
    }
    
}
ENode::~ENode(){
    for (auto child : this->children){
        delete child;
    }
}

ENode* ENode::addChild(ENode* child){
    this->children.push_back(child);
    return child;
}

ENode* ENode::getParent() const{
    return this->parent;
}

size_t ENode::getChildrenLength() const{
    return this->children.size();  
}