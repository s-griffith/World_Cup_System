#ifndef AVLMULTIVARIABLE_H
#define AVLMULTIVARIABLE_H

#include "AVLTree.h"

template<class T>
class MultiTree : public Tree<T> {
protected:
    class NodeExtraField : public Tree<T>::Node {
    public:
        T m_data2;
        T m_data3;

        NodeExtraField();
        NodeExtraField(const NodeExtraField&);
        NodeExtraField& operator=(const NodeExtraField& other);
        ~NodeExtraField() = default;
    };
public:
    NodeExtraField* m_root; //the original node* won't work with this tree - what do we want to do about it?
        //Maybe make tree with one pointer a signular class and then the general AVLTree won't have any internal fields
    
    MultiTree();
    ~MultiTree();
    MultiTree(const MultiTree& other);
    MultiTree& operator=(const MultiTree& other);

    bool insert(const T& data, const int id) override;

    typename NodeExtraField& search_specific_id(const int id) const;
    typename NodeExtraField& search_recursively(const int id, NodeExtraField* currentNode) const;
    T& search_and_return_data(const int id) const override;
};

#endif //AVLMULTIVARIABLE_H