#pragma once
#include <vector>


class ENode
{
protected:
    ENode* parent;
    std::vector<ENode*> children;
public:
    ENode(ENode* parent = nullptr);
    ~ENode();
    virtual ENode* addChild(ENode* child);
    ENode* getParent() const;
    size_t getChildrenLength() const;
};