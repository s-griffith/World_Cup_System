#ifndef WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H
#define WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H


#include "Node.h"
#include "ComplexNode.h"

/*
* Class NodeWithPointer, derived from parent class Node
* This class is used to create the separate nodes in the tree
*/
template <class T>
class NodeWithPointer : public Node<T> {
public:
    //Constructor
    NodeWithPointer();

    //The copy constructor, the assignment operator and the destructor
    NodeWithPointer(const NodeWithPointer&) = default;
    NodeWithPointer& operator=(const NodeWithPointer& other) = default;
    ~NodeWithPointer() = default;

    /*
     * Right-Right Rotation
     * @return - none
     */
    NodeWithPointer* rr_rotation(NodeWithPointer* node);

    /*
     * Right-Left Rotation
     * @return - none
     */
    NodeWithPointer* rl_rotation(NodeWithPointer* node);

    /*
     * Left-Left Rotation
     * @return - none
     */
    NodeWithPointer* lr_rotation(NodeWithPointer* node);

    /*
     * Left-Right Rotation
     * @return - none
     */
    NodeWithPointer* ll_rotation(NodeWithPointer* node);


    /*
     * Update height of the current node
     * @return - none
    */
    void update_height();

    /*
     * Update balance factor of the current node
     * @return - none
    */
    void update_bf();

    /*
     * Update balance factor of the current node
     * @return - none
    */
    void get_data_inorder(int* const array, int index) const;
    void printNode();
    void printData();
    void inorderWalkNode(bool flag);

protected:
    //Pointers to the parent node and the two child nodes
    NodeWithPointer* m_parent;
    NodeWithPointer* m_left;
    NodeWithPointer* m_right;
    typename ComplexNode<T>::ComplexNode* m_matchOtherTree;

    //Friend classes
    template <class K>
    friend class TreeExtraPointer;

    template <class N, class M>
    friend class Tree;

};

//-----------------------------------------Node Implementation-----------------------------------------------

//Node Constructor
template <class T>
NodeWithPointer<T>::NodeWithPointer() :
            Node<T>(),
            m_parent(nullptr),
            m_left(nullptr),
            m_right(nullptr),
            m_matchOtherTree(nullptr)
{}

//-----------------------------------------------Rotations---------------------------------------------------

template <class T>
void NodeWithPointer<T>::printNode() {
    int parent, left, right;
    if (m_parent == nullptr) {
        parent = -1;
    }
    else {
        parent = m_parent->m_id;
    }
    if (m_left == nullptr) {
        left = -1;
    }
    else {
        left = m_left->m_id;
    }
    if (m_right == nullptr) {
        right = -1;
    }
    else {
        right = m_right->m_id;
    }
    std::cout << "ID = " << Node<T>::m_id << ", Parent = " << parent << ", Left = " << left << ", Right = " << right << std::endl;
}

template <class T>
void NodeWithPointer<T>::printData() {
    std::cout << "Data = " << this->m_data << std::endl;
}

template <class T>
void NodeWithPointer<T>::inorderWalkNode(bool flag) {
    if (this != nullptr) {
        m_left->inorderWalkNode(flag);
        if (flag) {
            this->printNode();
        }
        else {
            this->printData();
        }
        m_right->inorderWalkNode(flag);
    }
}


//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
typename NodeWithPointer<T>::NodeWithPointer* NodeWithPointer<T>::ll_rotation(NodeWithPointer<T>* node)
{
    NodeWithPointer<T>* tmpToReturn = node;
    //Changing A->B to A->Parent
    m_left->m_parent = m_parent;
    //Changing Parent->B to Parent->A
    if (m_parent != nullptr) {
        if (m_parent->m_left == this) {
            m_parent->m_left = m_left;
        }
        else {
            m_parent->m_right = m_left;
        }
    }
    else {
        tmpToReturn = m_left;
    }
    //Changing B->Parent to B->A
    m_parent = m_left;
    //Changing Ar->A to Ar->B
    if (m_left->m_right != nullptr) {
        m_left->m_right->m_parent = this;
    }
    //Changing B->A to B->Ar
    m_left = m_left->m_right;
    //Changing A->Ar to A->B
    m_parent->m_right = this;
    return tmpToReturn;
}

//Right-Right tree rotation, on the node with balance factor of -2
template <class T>
typename NodeWithPointer<T>::NodeWithPointer* NodeWithPointer<T>::rr_rotation(NodeWithPointer<T>* node)
{
    NodeWithPointer<T>* tmpToReturn = node;
    m_right->m_parent = m_parent;
    if (m_parent != nullptr) {
        if (m_parent->m_right == this) {
            m_parent->m_right = m_right;
        }
        else {
            m_parent->m_left = m_right;
        }
    }
    else {
        tmpToReturn = m_right;
    }
    m_parent = m_right;
    if(m_right->m_left != nullptr) {
        m_right->m_left->m_parent = this;
    }
    m_right = m_right->m_left;
    m_parent->m_left = this;
    return tmpToReturn;
}


//Right-Left tree rotation, on the node with balance factor of -2
template <class T>
typename NodeWithPointer<T>::NodeWithPointer* NodeWithPointer<T>::rl_rotation(NodeWithPointer<T>* node)
{
    NodeWithPointer<T>* tmp = m_right->ll_rotation(node);
    tmp = rr_rotation(tmp);
    return tmp;
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
typename NodeWithPointer<T>::NodeWithPointer* NodeWithPointer<T>::lr_rotation(NodeWithPointer<T>* node)
{
    NodeWithPointer<T>* tmp = m_left->rr_rotation(node);
    tmp = ll_rotation(tmp);
    return tmp;
}

//----------------------------------------------Node Stats---------------------------------------------------

//Update the balance factor of the specific node
template <class T>
void NodeWithPointer<T>::update_bf()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    Node<T>::m_bf = heightLeft - heightRight;
}


//Update the subtree height of the specific node
template <class T>
void NodeWithPointer<T>::update_height()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    if (heightLeft >= heightRight) {
        Node<T>::m_height = heightLeft;
    }
    else {
        Node<T>::m_height = heightRight;
    }
}

template <class T>
void NodeWithPointer<T>::get_data_inorder(int* const array, int index) const
{
    if (m_left != nullptr) {
        m_left->get_data_inorder(array, index);
    }
    array[index++] = Node<T>::m_id;
    if (m_right != nullptr) {
        m_right->get_data_inorder(array, index);
    }
}

//-----------------------------------------------------------------------------------------------------------

#endif //WORLD_CUP_SYSTEM_NODEADDITIONALPOINTER_H
