#ifndef AVLMULTIVARIABLE_H
#define AVLMULTIVARIABLE_H

#include "AVLTree.h"
#include "Node.h"
#include "ComplexNode.h"
#include "Exception.h"

template<class T> //does this use the current complexNode?? check by trying something else
class MultiTree : public Tree<ComplexNode<T>, T> {
    template<class M>
    friend class ComplexNode<T>::ComplexNode;
public:
    //ComplexNode<T>* m_root; //Already has m_node from Tree class - what should we do about it?
    
    MultiTree()=default;
    ~MultiTree();
    MultiTree(const MultiTree& other);
    MultiTree& operator=(const MultiTree& other);

    void insert(const T& data, const int id, const int goals, const int cards);

    //typename Node<T>::Node<T>& search_specific_id(const int id) const override;
    //typename Node<T>::Node<T>& search_recursively(const int id, ComplexNode<T>* currentNode) const;
    //T& search_and_return_data(const int id) const override;
};

//template<template<class T> class ComplexNode, class T>
//MultiTree<ComplexNode, T>::MultiTree() : Tree<ComplexNode::ComplexNode<T>, T>::Tree() {}

template<class T>
MultiTree<T>::~MultiTree() {
    this->Tree<ComplexNode<T>, T>::destroy_tree(Tree<ComplexNode<T>, T>::m_node);
}

template<class T>
void MultiTree<T>::insert(const T& data, const int id, const int goals, const int cards) {
        //If this is the first node in the tree:
    if (Tree<ComplexNode<T>, T>::m_node->m_height == -1) {
        Tree<ComplexNode<T>, T>::m_node->m_data = data;
        Tree<ComplexNode<T>, T>::m_node->m_id = id;
        Tree<ComplexNode<T>, T>::m_node->m_height++;
        Tree<ComplexNode<T>, T>::m_node->m_goals = goals;
        Tree<ComplexNode<T>, T>::m_node->m_cards = cards;
        return;
    }
    //Find the proper location of the new node (when it's not the first):
    ComplexNode<T>* x = Tree<ComplexNode<T>, T>::m_node;
    ComplexNode<T>* y = nullptr;
    while (x != nullptr) {
        y = x;
        if (x->m_id == id) {
            throw InvalidID(); //node with that id already exists - invalid operation
        }
        //maybe make this into a switch case
        if (goals < x->m_goals) {
            x = x->m_left;
        }
        else if (goals == x->m_goals) {
            if (cards > x->m_cards) {
                x = x->m_left;
            }
            else if (cards == x->m_cards) {
                if (id < x->m_goals) {
                    x = x->m_left;
                }
            }
        }
        else {
            x = x->m_right;
        }
    }
    //Create the new node and add it to the tree:
    ComplexNode<T>* node = nullptr;
    try{
        node = new ComplexNode<T>;
    }
    catch(const std::bad_alloc& e)
    {
        delete node;
        throw e;
    }
    node->m_parent = y;
    node->m_left = nullptr;
    node->m_right = nullptr;
    node->m_data = data;
    node->m_goals = goals;
    node->m_cards = cards;
    node->m_id = id;
    node->m_height = 0;
    if (node->m_goals < y->m_goals) {
        y->m_left = node;
    }
    else if (node->m_goals == y->m_goals) {
        if (node->m_cards > y->m_cards) {
            y->m_left = node;
        }
        else if (node->m_cards == y->m_cards) {
            if (node->m_id < y->m_id) {
                y->m_left = node;
            }
        }
    }
    else {
        y->m_right = node;
    }
    this->rebalance_tree(node->m_parent);
}



#endif //AVLMULTIVARIABLE_H