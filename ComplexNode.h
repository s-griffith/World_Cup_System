#ifndef COMPLEXNODE_H
#define COMPLEXNODE_H

#include "Node.h"
#include "AVLMultiVariable.h"


template <class T>
class ComplexNode : public Node<T> {
private:
    T m_data2;
    T m_data3;
    template<template<class M> class ComplexNode, class M>
    friend class MultiTree;
public:
    ComplexNode();
    ComplexNode(const ComplexNode&);
    ComplexNode& operator=(const ComplexNode& other);
    ~ComplexNode() = default;


    //maybe add sort()?
};

template<class T>
ComplexNode<T>::ComplexNode(){
    std::cout << "In correct constructor" <<std::endl;
}

#endif //COMPLEXNODE_H