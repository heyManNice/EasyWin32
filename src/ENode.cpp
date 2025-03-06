#include "ENode.hpp"

ENode::ENode(ENode* parent){
    this->parent = parent;
    this->parent->children.push_back(this);
}
ENode::~ENode(){
    for (auto child : this->children){
        delete child;
    }
}