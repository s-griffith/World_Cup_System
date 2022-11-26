#ifndef AVLTREE_h
#define AVLTREE_h

#include <iostream>
#include <string>
#include <new>
#include <memory>
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
        T m_data;
        //The height of the subtree of this node
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
         * Right-Right Rotation
         * @return - none
         */
        void rr_rotation();

        /*
         * Right-Left Rotation
         * @return - none
         */
        void rl_rotation();

        /*
         * Left-Left Rotation
         * @return - none
         */
        void ll_rotation();

        /*
         * Left-Right Rotation
         * @return - none
         */
        void lr_rotation();

        /*
         * Make the node a leaf without breaking the sorted tree
         * @return - a pointer to the node from which the tree is no longer balanced
         */
        Node* make_node_leaf();

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
    void destroy_tree(Node* currentNode);

    /*
     * Recursively copy full tree
     * @return - none
     */
    void copy_tree(Node* currentNode, Node* otherNode);

    /*
     * Insert new node with data, according to the id given
     * @param - New data to insert and the ID of the new node
     * @return - bool
     */
    bool insert(const T& data, const int id);

    /*
     * Remove node according to the id given
     * @param - The ID of the node that needs to be removed
     * @return - none
     */
    void remove(const int id);
    void rebalance_tree(Node* currentNode);

    /*
     * Search for node with specific data, according to the id given
     * @param - The ID of the requested node
     * @return - none
     */
    typename Tree<T>::Node::Node& search_specific_id(const int id) const;
    typename Tree<T>::Node::Node& search_recursively(const int id, const Node* currentNode) const;

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
    //Copy first node
    m_node = other.m_node;
    //Copy existing tree to new tree
    copy_tree(m_node, other.m_node);
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
        destroy_tree(currentNode->m_left);
        destroy_tree(currentNode->m_right);
        delete tmpNode;
    }
}


//Copy tree recursively
template <class T>
void Tree<T>::copy_tree(Node* currentNode, Node* otherNode)
{
    if (otherNode->m_left != nullptr) {
        //Create empty new node
        try {
            currentNode->m_left = new Node;
            currentNode->m_left->m_data = otherNode->m_left->m_data;
            copy_tree(currentNode->m_left, otherNode->m_left);
        }
        catch (const std::bad_alloc& e) {
            delete currentNode->m_left;
            throw std::bad_alloc();
        }
    }
    if (otherNode->get_right() != nullptr) {
        //Create empty new node
        try {
            currentNode->m_right = new Node;
            currentNode->m_right->m_data = otherNode->m_right->m_data;
            copy_tree(currentNode->m_right, otherNode->m_right);
        }
        catch (const std::bad_alloc& e) {
            delete currentNode->m_right;
            throw std::bad_alloc();
        }
    }
}

template<class T>
bool Tree<T>::insert(const T& data, const int id) {
    //If this is the first node in the tree:
    if (m_node->m_data == NULL) {
        m_node->m_data = data; //Why is node.data a shared_ptr of a shared_ptr??!?!?
        m_node->m_id = id;
        m_node->m_height++;
        return true;
    }
    //Find the proper location of the new node (when it's not the first):
    Node* x = m_node;
    Node* y = nullptr;
    while (x != nullptr) {
        y = x;
        if (x->m_id == id) {
            return false; //node with that id already exists - invalid operation
        }
        if (id < x->m_id) {
            x = x->m_left;
        }
        else {
            x = x->m_right;
        }
    }
    //Create the new node and add it to the tree:
    Node* node = nullptr;
    try{
        node = new Node;
    }
    catch(const std::bad_alloc& e)
    {
        delete node;
        return false;
    }
    node->m_parent = y;
    node->m_left = nullptr;
    node->m_right = nullptr;
    node->m_data = data;
    node->m_id = id;
    if (id < y->m_id) {
        y->m_left = node;
    }
    else {
        y->m_right = node;
    }
    rebalance_tree(node->m_parent);
    return true;
}

//Remove node according to the id provided
template <class T>
void Tree<T>::remove(const int id)
{
    Node* toRemove = search_specific_id(id);
    Node* nodeToFix = toRemove->make_node_leaf();
    delete toRemove;
    //Go up the tree and check the balance factors and complete needed rotations
    rebalance_tree(nodeToFix);
}


//Re-balance tree after node removal
template <class T>
void Tree<T>::rebalance_tree(Node* currentNode) {
    if (currentNode == nullptr) {
        return;
    }
    currentNode->update_height();
    currentNode->update_bf();
    if (currentNode->m_bf > 1 || currentNode->m_bf < -1) {
        if (currentNode->m_bf == 2) {
            if (currentNode->m_left->m_bf == -1) {
                currentNode->lr_rotation();
            }
            else {
                currentNode->ll_rotation();
            }
        }
        else if (currentNode->m_bf == -2) {
            if (currentNode->m_right->m_bf == 1) {
                currentNode->rl_rotation();
            }
            else {
                currentNode->rr_rotation();
            }
        }
        if (currentNode->m_parent->m_left != nullptr) {
            currentNode->m_parent->m_left->update_height();
            currentNode->m_parent->m_left->update_bf();
        }
        if (currentNode->m_parent->m_right != nullptr) {
            currentNode->m_parent->m_right->update_height();
            currentNode->m_parent->m_right->update_bf();
        }
    }
    rebalance_tree(currentNode->m_parent);
}

//Combine??
//Search and return node with specific ID
template <class T>
typename Tree<T>::Node::Node& Tree<T>::search_specific_id(const int id) const
{
    return search_recursively(id, m_node);
}


//Search and return node with specific ID recursively
template <class T>
typename Tree<T>::Node::Node& Tree<T>::search_recursively(const int id, const Node* currentNode) const
{
    if (currentNode == nullptr) {
        throw NodeNotFound();
    }
    if (currentNode->m_id == id) {
        return currentNode;
    }
    if (currentNode->m_id > id) {
        return search_recursively(id, currentNode->m_right);
    }
    return search_recursively(id, currentNode->m_left);
}

//---------------------------------------------------------------------------------------------------------

//--------------------------------------------Node Class---------------------------------------------------

//Node Constructor
template <class T>
Tree<T>::Node::Node() : m_parent(nullptr), m_left(nullptr), m_right(nullptr), m_data(nullptr), m_height(-1), 
    m_id(0), m_bf(0) {}

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

//Right-Right tree rotation, on the node with balance factor of -2
template <class T>
void Tree<T>::Node::rr_rotation()
{
    m_right->m_parent = m_parent;
    if (m_parent != nullptr) {
        if (m_parent->m_right == this) {
            m_parent->m_right = m_right;
        }
        else {
            m_parent->m_left = m_right;
        }
    }
    m_parent = m_right;
    if(m_right->m_left != nullptr) {
        m_right->m_left->m_parent = this;
    }
    m_right = m_right->m_left;
    m_parent->m_left = this;
}

template <class T>
void Tree<T>::Node::rl_rotation()
{
    m_left->ll_rotation();
    rr_rotation();
}

template <class T>
void Tree<T>::Node::lr_rotation()
{
    m_right->rr_rotation();
    ll_rotation();
}

/*
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
*/

//Make the current node a leaf (maintaining sort)
template <class T>
typename Tree<T>::Node::Node* Tree<T>::Node::make_node_leaf()
{
    //Node to be deleted is already a leaf
    if (m_left == nullptr && m_right == nullptr) {
        return m_parent;
    }
    //Node to be deleted has one child
    if (m_left == nullptr || m_right == nullptr) {
        Node* tmpChild;
        if (m_left != nullptr) {
            tmpChild = m_left;
        }
        else {
            tmpChild = m_right;
        }
        //Connect child to parent
        tmpChild->m_parent = m_parent;
        if (m_parent != nullptr) {
            if (m_parent->m_left == this) {
                m_parent->m_left = tmpChild;
            }
            else {
                m_parent->m_right = tmpChild;
            }
        }
        return m_parent;
    }
    //Node to be deleted has two children
    Node* successor = m_right;
    while (successor->m_left != nullptr) {
        successor = successor->m_left;
    }
    Node* parentToReturn;
    if (successor != m_right) {
        parentToReturn = successor->m_parent;
    }
    else {
        parentToReturn = successor;
    }
    if (successor->m_right == nullptr) {
        if (successor->m_parent->m_right == successor) {
            successor->m_parent->m_right = nullptr;
        }
        else {
            successor->m_parent->m_left = nullptr;
        }
    }
    else if (m_right != successor) {
        successor->m_right->m_parent = successor->m_parent;
        if (successor->m_parent->m_right == successor) {
            successor->m_parent->m_right = successor->m_right;
        }
        else {
            successor->m_parent->m_left = successor->m_right;
        }
    }
    //Switch between successor and current node
    successor->m_parent = m_parent;
    if (m_parent != nullptr) {
        if (m_parent->m_right == this) {
            m_parent->m_right = successor;
        }
        else {
            m_parent->m_left = successor;
        }
    }
    successor->m_left = m_left;
    if (m_left != nullptr) {
        m_left->m_parent = successor;
    }
    if (successor != m_right) {
        successor->m_right = m_right;
        if (m_right != nullptr) {
            m_right->m_parent = successor;
        }
    }
    return parentToReturn;
}


//Update the balance factor of the specific node
template <class T>
void Tree<T>::Node::update_bf()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    m_bf = heightLeft - heightRight;
}


//Update the subtree height of the specific node
template <class T>
void Tree<T>::Node::update_height()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    if (heightLeft >= heightRight) {
        m_height = heightLeft;
    }
    else {
        m_height = heightRight;
    }
}

//---------------------------------------------------------------------------------------------------------


#endif //AVLTREE_h