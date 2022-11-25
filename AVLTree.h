#ifndef AVLTREE_h
#define AVLTREE_h

#include <iostream>
#include <string>
#include <new>
#include "Exception.h"

template <class T>
class Tree {
    /*
    * Class Tree::Node
    * This class is used to create the separate nodes in the tree
    */
    class Node{
    public:
        //Pointers to the parent node and the two child nodes
        Node* m_parent;
        Node* m_left;
        Node* m_right;
        //Shared ptr of the data that the tree holds
        std::shared_ptr<T> m_data;
        //The height of the sub-tree of this node
        int m_height;
        //Balance factor of node
        int m_bf;
        //The key by which the tree is sorted
        int m_id;

        //Constructor
        Node();
        /*
         * Use the default methods for the copy constructor, the assignment operator and the destructor
        */
        Node(const Node&) = default;
        Node& operator=(const Node& other) = default;
        ~Node() = default;

        /*
         * RR Rotation
         * @return - none
         */
        void rr_rotation();

        /*
         * RL Rotation
         * @return - none
         */
        void rl_rotation();

        /*
         * LL Rotation
         * @return - none
         */
        void ll_rotation();

        /*
         * LR Rotation
         * @return - none
         */
        void lr_rotation();
    };
public:
    //Tree Constructor
    Tree();

    //Tree Copy constructor, assignment operator and destructor
    ~Tree();
    Tree(const Tree& other);
    Tree& operator=(const Tree& other);

    /*
     * Recursively destroy full tree
     * @return - none
     */
    void destroy_tree();

    /*
     * Recursively copy full tree
     * @return - none
     */
    void copy_tree();

    /*
     * Insert new node with data, according to the id given
     * @param - New data to insert and the ID of the new node
     * @return - none
     */
    void insert(const T& data, const int id);

    /*
     * Remove node according to the id given
     * @param - The ID of the node that needs to be removed
     * @return - none
     */
    void remove(const int id);
    Node* check_bf_remove(Node* currentNode);

    /*
     * Search for node with specific data, according to the id given
     * @return - none
     */
    Node& search_specific_id(const int id) const;
    Node& search_recursively(const int id, const Node& currentNode) const;

    /*
     * Calculate the balance factor
     * @return - none
     */
    int calculate_balance_factor(const Tree& currentTree) const;

protected:
    Node* m_node;
};

//--------------------------------------------Tree Class---------------------------------------------------

//Tree Constructor
template <class T>
Tree<T>::Tree()
{
    //Creating empty beginning node
    try {
        m_node = new Node;
    }
    catch (const std::bad_alloc& e) {
        delete m_node;
        throw std::bad_alloc();
    }
}


//Tree Destructor
template <class T>
Tree<T>::~Tree()
{
    destroy_tree(m_node);
}


//Tree Copy Constructor
template <class T>
Tree<T>::Tree(const Tree& other)
{
    //Create empty beginning node
    try {
        m_node = new Node;
    }
    catch (const std::bad_alloc& e) {
        delete m_node;
        throw std::bad_alloc();
    }
    //Copy existing tree to new tree
    copy_tree(m_node, other);
}


//Tree Assignment Operator
template <class T>
typename Tree<T>::Tree& Tree<T>::operator=(const Tree& other)
{
    if (this == &other) {
        return *this;
    }
    //Copy the other tree to a new tree
    Tree newTree(other);
    //Create temporary tree variable
    Node* tmp = newTree.m_node;
    //Copy the current tree to the temporary tree
    newTree.m_node = m_node;
    //Copy the other tree to the current tree
    m_node = tmp;
    return *this;
    //The temporary tree, pointing to the old current tree will now be destroyed
}


//Destroy tree recursively
template <class T>
void Tree<T>::destroy_tree(Node* currentNode)
{
    Node* tmpNode = currentNode;
    if (tmpNode != nullptr) {
        destroyTree(currentNode->m_left);
        destroyTree(currentNode->m_right);
        delete tmpNode;
    }
}


//Copy tree recursively-----------------------------------------------------------------UNFINISHED
template <class T>
void Tree<T>::copy_tree(Node* currentNode, const Tree& other)
{
    if (other.get_left() != nullptr) {
        //Create empty new node
        try {
            currentNode->m_left = new Node;
            currentNode->m_left->m_data = other.get_left_data();
            copy_tree(currentNode->m_left, other.get_left())
        }
        catch (const std::bad_alloc& e) {
            delete currentNode->m_left;
            throw std::bad_alloc();
        }
    }
    if (other->get_right() != nullptr) {
        //Create empty new node
        try {
            currentNode->m_right = new Node;
            currentNode->m_right->m_data = other.get_right_data();
            copy_tree(currentNode->m_right, other.get_right())
        }
        catch (const std::bad_alloc& e) {
            delete currentNode->m_right;
            throw std::bad_alloc();
        }
    }
}

//-----------------------------------------------------------------------Unfinished
template <class T>
void Tree<T>::remove(const int id)
{
    Node* toRemove = roll_down(search_specific_id(id));
    //Change parent pointer to nullptr and update parent balance factor
    Node* parent = toRemove->m_parent;
    if (parent->m_left == toRemove) {
        parent->m_left = nullptr;
        parent->m_bf--;
    }
    else {
        parent->m_right = nullptr;
        parent->m_bf++;
    }
    delete toRemove;
    //Go up the tree and check the balance factors and complete needed rotations
    check_bf_remove(parent);
}


//-----------------------------------------------------------------------Unfinished
template <class T>
Node* Tree<T>::check_bf_remove(Node* currentNode) {
    if (currentNode->m_parent->m_left == currentNode) {
        currentNode->m_parent->m_bf--;
    }
    else {
        currentNode->m_parent->m_bf++;
    }
    if (currentNode->m_parent->m_bf == 2) {
        if (currentNode->m_parent->m_left->m_bf == -1) {
            currentNode->m_parent->lr_rotation();
            currentNode->m_parent->m_left->m_bf++;
            currentNode->m_parent->m_bf = 0;
            currentNode->m_parent->m_right->m_bf = -1;
        }
        else {
            currentNode->m_parent->ll_rotation();
            currentNode->m_parent->m_bf = 0;
            currentNode->m_parent->m_right->m_bf = 0;
        }
    }
    if (currentNode->m_parent->m_bf == -2) {
        if (currentNode->m_parent->m_right->m_bf == 1) {
            currentNode->m_parent->rl_rotation();
            currentNode->m_parent->m_left->m_bf = 1;
            currentNode->m_parent->m_bf = 0;
            currentNode->m_parent->m_right->m_bf = 0;
        }
        else {
            currentNode->m_parent->rr_rotation();
            currentNode->m_parent->m_left->m_bf = 0;
            currentNode->m_parent->m_bf = 0;
            currentNode->m_parent->m_right->m_bf = -1;
        }
    }
}





template <class T>
Node& Tree<T>::search_specific_id(const int id) const
{
    return search_recursively(id, *m_node);
}


template <class T>
Node& Tree<T>::search_specific_id(const int id, const Node& currentNode) const
{
    if (currentNode == nullptr) {
        throw NodeNotFound();
    }
    if (currentNode.m_id == id) {
        return currentNode;
    }
    if (currentNode.m_id > id) {
        return search_recursively(id, currentNode.m_right);
    }
    return search_recursively(id, currentNode.m_left);
}

//---------------------------------------------------------------------------------------------------------

//--------------------------------------------Node Class---------------------------------------------------

//Node Constructor
template <class T>
Tree<T>::Node::Node()
{
    m_parent = nullptr;
    m_left = nullptr;
    m_right = nullptr;
    m_data = nullptr;
    m_height = 0;
    m_id = 0;
    m_bf = 0;
}

//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
void Tree<T>::Node::ll_rotation()
{
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
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
void Tree<T>::Node::lr_rotation()
{
    //Changing B->A to B->Parent
    m_left->m_right->m_parent = m_parent;
    //Changing Parent->C to Parent->B
    if (m_parent != nullptr) {
        if (m_parent->m_left == this) {
            m_parent->m_left = m_left->m_right;
        }
        else {
            m_parent->m_right = m_left->m_right;
        }
    }
    //Changing C->Parent to C->B
    m_parent = m_left->m_right;
    //Changing A->B to A->Bl
    m_left->m_right = m_left->m_right->m_left;
    //Changing Bl->B to Bl->A
    m_parent->m_left->m_parent = m_left;
    //Changing B->Bl to B->A
    m_parent->m_left = m_left;
    //Changing A->C to A->B
    m_left->m_parent = m_parent;
    //Changing C->A to C->Br
    m_left = m_parent->m_right;
    //Changing Br->B to Br->C
    if (m_parent->m_right != nullptr) {
        m_parent->m_right->m_parent = this;
    }
    //Changing B->Br to B->C
    m_parent->m_right = this;
}

//---------------------------------------------------------------------------------------------------------


#endif //AVLTREE_h