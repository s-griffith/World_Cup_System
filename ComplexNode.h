#ifndef COMPLEXNODE_H
#define COMPLEXNODE_H

#include "Node.h"

/*
* Class Complex Node : Node
* This class is used to create separate nodes in the tree, sorted based on three keys:
*       the number of goals the player scored, the number of cards they received and their player ID
*/
template <class T>
class ComplexNode : Node<T> {
public:
    //Constructors
    ComplexNode();
    ~ComplexNode() = default;
    ComplexNode(const ComplexNode&);
    ComplexNode& operator=(const ComplexNode& other);

private:

    /*
     * Left-Right Rotation
     * @return - none
     */
    ComplexNode* ll_rotation(ComplexNode* node);

    /*
     * Right-Right Rotation
     * @return - none
     */
    ComplexNode* rr_rotation(ComplexNode* node);

    /*
     * Right-Left Rotation
     * @return - none
     */
    ComplexNode* rl_rotation(ComplexNode* node);

    /*
     * Left-Left Rotation
     * @return - none
     */
    ComplexNode* lr_rotation(ComplexNode* node);

    /*
     * Update balance factor of the current node
     * @return - none
    */
    void update_bf();
    
    /*
     * Update height of the current node
     * @return - none
    */
    void update_height();

    int get_data_inorder(int* const array, int index) const;

    //Helper functions to print
    void inorderWalkNode(bool flag);
    void printNode();
    void printData();

    //Friend classes
    template <class M>
    friend class MultiTree;

    template <class K>
    friend class TreeExtraPointer;

    template <class ComplexNode, class N>
    friend class Tree;

    //Pointers to the parent node and the two child nodes
    ComplexNode<T>* m_parent;
    ComplexNode<T>* m_left;
    ComplexNode<T>* m_right;
    //The goals the player scored and the cards they received
    int m_goals;
    int m_cards;
    
};


//-----------------------------------------Constructors--------------------------

template <class T>
ComplexNode<T>::ComplexNode() :
        Node<T>(),
        m_parent(nullptr),
        m_left(nullptr),
        m_right(nullptr),
        m_goals(0),
        m_cards(0)
{}


template<class T>
ComplexNode<T>::ComplexNode(const ComplexNode&) {

}


template<class T>
ComplexNode<T>& ComplexNode<T>::operator=(const ComplexNode& other) {

}


//--------------------------------------------Rotations---------------------------------------------------

//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
typename ComplexNode<T>::ComplexNode* ComplexNode<T>::ll_rotation(ComplexNode<T>* node)
{
    ComplexNode<T>* tmpToReturn = node;
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
typename ComplexNode<T>::ComplexNode* ComplexNode<T>::rr_rotation(ComplexNode<T>* node)
{
    ComplexNode<T>* tmpToReturn = node;
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
typename ComplexNode<T>::ComplexNode* ComplexNode<T>::rl_rotation(ComplexNode<T>* node)
{
    ComplexNode<T>* tmp = m_right->ll_rotation(node);
    tmp = rr_rotation(tmp);
    return tmp;
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
typename ComplexNode<T>::ComplexNode* ComplexNode<T>::lr_rotation(ComplexNode<T>* node)
{
    ComplexNode<T>* tmp = m_left->rr_rotation(node);
    tmp = ll_rotation(tmp);
    return tmp;
}


//----------------------------------------------Node Stats---------------------------------------------------

//Update the balance factor of the specific node
template <class T>
void ComplexNode<T>::update_bf()
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
void ComplexNode<T>::update_height()
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
int ComplexNode<T>::get_data_inorder(int* const array, int index) const
{
    if (this != nullptr) {
        index = m_left->get_data_inorder(array, index);
        array[index++] = this->m_id;
        index = m_right->get_data_inorder(array, index);
    }
    return index;
}

//-----------------------------------------FUNCTIONS FOR TESTING--------------------------

template <class T>
void ComplexNode<T>::printNode() {
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
    std::cout << "ID = " << Node<T>::m_id << ", Parent = " << parent << ", Left = " 
            << left << ", Right = " << right << std::endl;
}


template <class T>
void ComplexNode<T>::printData() {
    std::cout << "ID = " << Node<T>::m_id << std::endl;
}


template <class T>
void ComplexNode<T>::inorderWalkNode(bool flag) {
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

//-----------------------------------------------------------------------------------------------------------

#endif //COMPLEXNODE_H