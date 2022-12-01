#ifndef AVLMULTIVARIABLE_H
#define AVLMULTIVARIABLE_H

#include "AVLTree.h"
#include "Node.h"
#include "ComplexNode.h"

template<template<class T> class ComplexNode, class T> //does this use the current complexNode?? check by trying something else
class MultiTree : public Tree<ComplexNode<T>, T> {
    template<class M>
    friend class ComplexNode;
public:
    ComplexNode<T>* m_root;
    
    MultiTree() = default;
    ~MultiTree();
    MultiTree(const MultiTree& other);
    MultiTree& operator=(const MultiTree& other);

    void destroy_tree(ComplexNode<T>* currentNode);

    bool insert(const T& data, const int id) override;

    typename Node<T>::Node<T>& search_specific_id(const int id) const override;
    typename Node<T>::Node<T>& search_recursively(const int id, Node<T>* currentNode) const override;
    T& search_and_return_data(const int id) const override;
};

template<template<class T> class ComplexNode, class T>
MultiTree<ComplexNode, T>::~MultiTree() {
    MultiTree<ComplexNode, T>::destroy_tree(m_root);
    Tree<ComplexNode, T>::destroy_tree(dynamic_cast<Node<T>*>(m_root));
}

/*template<template<class T> class ComplexNode, class T>
MultiTree<ComplexNode, T>::MultiTree() {
    //Creating empty beginning node
    try {
        m_root = new ComplexNode;
    }
    catch (const std::bad_alloc& e) {
        delete m_root;
        throw std::bad_alloc();
    }
}*/



#endif //AVLMULTIVARIABLE_H