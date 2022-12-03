#ifndef WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H
#define WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H

#include "Node.h"

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
    NodeWithPointer(const NodeWithPointer&);
    NodeWithPointer& operator=(const NodeWithPointer& other);
    ~NodeWithPointer() = default;
protected:
    typename Node<T>::Node* m_matchOtherTree;
};

//-----------------------------------------Node Implementation-----------------------------------------------

//Node Constructor
template <class T>
NodeWithPointer<T>::NodeWithPointer() : Node<T>(), m_matchOtherTree(nullptr)
{}



#endif //WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H
