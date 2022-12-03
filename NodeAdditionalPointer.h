#ifndef WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H
#define WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H

#include "Node.h"
#include "ComplexNode.h"
#include "AVLTree.h"
#include "AVLTreeExtraPointer.h"

/*
* Class NodeWithPointer, derived from parent class Node
* This class is used to create the separate nodes in the tree
*/
template <class T>
class NodeWithPointer : public Node<T> {
public:
    //Constructor
    NodeWithPointer();
    /*
     * The copy constructor, the assignment operator and the destructor
    */
    NodeWithPointer(const NodeWithPointer&) = default;
    NodeWithPointer& operator=(const NodeWithPointer& other) = default;
    ~NodeWithPointer() = default;
protected:
    typename ComplexNode<T>::ComplexNode* m_matchOtherTree;

    template <class K>
    friend class TreeByScore;

    template <class N, class M>
    friend class Tree;
};

//-----------------------------------------Node Implementation-----------------------------------------------

//Node Constructor
template <class T>
NodeWithPointer<T>::NodeWithPointer() : Node<T>(), m_matchOtherTree(nullptr)
{}



#endif //WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H
