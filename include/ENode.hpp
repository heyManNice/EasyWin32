#pragma once
#include <vector>


class ENode
{
private:
    friend class EWidget;
    ENode* parent;
    std::vector<ENode*> children;
public:
    ENode(ENode* parent = nullptr);
    ~ENode();
    ENode* addChild(ENode* child);
    ENode* getParent() const;
    size_t getChildrenLength() const;
};