#ifndef AVLTREE_h
#define AVLTREE_h

#include <iostream>
#include <string>
#include <new>
#include <memory>
#include "Exception.h"

//AVL tree template class
template <class T>
class Tree {
protected:
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
        //The data that the tree holds
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
         * The copy constructor, the assignment operator and the default destructor
        */
        Node(const Node&);
        Node& operator=(const Node& other);
        virtual ~Node() = default;

        /*
         * Right-Right Rotation
         * @return - none
         */
        typename Tree<T>::Node::Node* rr_rotation(Node* node);

        /*
         * Right-Left Rotation
         * @return - none
         */
        typename Tree<T>::Node::Node* rl_rotation(Node* node);

        /*
         * Left-Left Rotation
         * @return - none
         */
        typename Tree<T>::Node::Node* lr_rotation(Node* node);

        /*
         * Left-Right Rotation
         * @return - none
         */
        typename Tree<T>::Node::Node* ll_rotation(Node* node);


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

        void inorderWalkNode(bool flag);
        void printNode();
        void printData();

    };

    //The tree's root node
    Node* m_node;

public:
    //Tree Constructor
    Tree();

    //Tree Copy constructor, assignment operator and destructor
    virtual ~Tree();
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
    virtual bool insert(const T& data, const int id);

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
    typename Tree<T>::Node::Node& search_recursively(const int id, Node* currentNode) const;
    virtual T& search_and_return_data(const int id) const;

    /*
    * Make the node a leaf without breaking the sorted tree
    * @return - a pointer to the node from which the tree is no longer balanced
    */
    typename Tree<T>::Node::Node* make_node_leaf(Node* node);

//-----------------------------------------FUNCTIONS FOR TESTING--------------------------

    void inorderWalk(bool flag);
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
    m_node->m_data = other.m_node->m_data;
    m_node->m_height = other.m_node->m_height;
    m_node->m_bf = other.m_node->m_bf;
    m_node->m_id = other.m_node->m_id;
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
            currentNode->m_left->m_parent = currentNode;
            currentNode->m_left->m_data = otherNode->m_left->m_data;
            currentNode->m_left->m_height = otherNode->m_left->m_height;
            currentNode->m_left->m_bf = otherNode->m_left->m_bf;
            currentNode->m_left->m_id = otherNode->m_left->m_id;
            copy_tree(currentNode->m_left, otherNode->m_left);
        }
        catch (const std::bad_alloc& e) {
            delete currentNode->m_left;
            throw std::bad_alloc();
        }
    }
    if (otherNode->m_right != nullptr) {
        //Create empty new node
        try {
            currentNode->m_right = new Node;
            currentNode->m_right->m_parent = currentNode;
            currentNode->m_right->m_data = otherNode->m_right->m_data;
            currentNode->m_right->m_height = otherNode->m_right->m_height;
            currentNode->m_right->m_bf = otherNode->m_right->m_bf;
            currentNode->m_right->m_id = otherNode->m_right->m_id;
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
    if (m_node->m_height == -1) {
        m_node->m_data = data;
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
    node->m_height = 0;
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
    Node* toRemove = &(search_specific_id(id));
    Node* nodeToFix = make_node_leaf(toRemove);
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
                m_node = currentNode->lr_rotation(m_node);
            }
            else {
                m_node = currentNode->ll_rotation(m_node);
            }
        }
        else if (currentNode->m_bf == -2) {
            if (currentNode->m_right->m_bf == 1) {
                m_node = currentNode->rl_rotation(m_node);
            }
            else {
                m_node = currentNode->rr_rotation(m_node);
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


//Combine??
//Search and return node with specific ID
template <class T>
T& Tree<T>::search_and_return_data(const int id) const
{
    return search_recursively(id, m_node).m_data;
}


//Search and return node with specific ID recursively
template <class T>
typename Tree<T>::Node::Node& Tree<T>::search_recursively(const int id, Node* currentNode) const
{
    if (currentNode == nullptr) {
        throw NodeNotFound();
    }
    if (currentNode->m_id == id) {
        return *(currentNode);
    }
    if (currentNode->m_id < id) {
        return search_recursively(id, currentNode->m_right);
    }
    return search_recursively(id, currentNode->m_left);
}

//---------------------------------------------------------------------------------------------------------


template <class T>
void Tree<T>::Node::printNode() {
    int parent, left, right;
    if (m_parent == nullptr) {
        parent = -1;
    }
    else {
        parent = m_parent->m_data;
    }
    if (m_left == nullptr) {
        left = -1;
    }
    else {
        left = m_left->m_data;
    }
    if (m_right == nullptr) {
        right = -1;
    }
    else {
        right = m_right->m_data;
    }
    printf("Data = %d, Parent = %d, Left = %d, Right = %d\n", m_data, parent, left, right);
}

template <class T>
void Tree<T>::Node::printData() {
    printf("Data = %d\n", m_data);
}

template <class T>
void Tree<T>::Node::inorderWalkNode(bool flag) {
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

template <class T>
void Tree<T>::inorderWalk(bool flag) {
    m_node->inorderWalkNode(flag);
}

//--------------------------------------------Node Class---------------------------------------------------

//Node Constructor
template <class T>
Tree<T>::Node::Node() : m_parent(nullptr), m_left(nullptr), m_right(nullptr), m_height(-1),
    m_id(0), m_bf(0)
    {}



//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
typename Tree<T>::Node::Node* Tree<T>::Node::ll_rotation(Node* node)
{
    Node * tmpToReturn = node;
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
typename Tree<T>::Node::Node* Tree<T>::Node::rr_rotation(Node* node)
{
    Node* tmpToReturn = node;
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
typename Tree<T>::Node::Node* Tree<T>::Node::rl_rotation(Node* node)
{
    Node* tmp = m_right->ll_rotation(node);
    tmp = rr_rotation(tmp);
    return tmp;
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
typename Tree<T>::Node::Node* Tree<T>::Node::lr_rotation(Node* node)
{
    Node* tmp = m_left->rr_rotation(node);
    tmp = ll_rotation(tmp);
    return tmp;
}


//Make the current node a leaf (maintaining sort)
template <class T>
typename Tree<T>::Node::Node* Tree<T>::make_node_leaf(Node* node)
{
    //Node to be deleted is already a leaf
    if (node->m_left == nullptr && node->m_right == nullptr) {
        return node->m_parent;
    }
    //Node to be deleted has one child
    if (node->m_left == nullptr || node->m_right == nullptr) {
        Node* tmpChild;
        if (node->m_left != nullptr) {
            tmpChild = node->m_left;
        }
        else {
            tmpChild = node->m_right;
        }
        //Connect child to parent
        tmpChild->m_parent = node->m_parent;
        if (node->m_parent != nullptr) {
            if (node->m_parent->m_left == node) {
                node->m_parent->m_left = tmpChild;
            }
            else {
                node->m_parent->m_right = tmpChild;
            }
        }
        return node->m_parent;
    }
    //Node to be deleted has two children
    Node* successor = node->m_right;
    while (successor->m_left != nullptr) {
        successor = successor->m_left;
    }
    Node* parentToReturn;
    if (successor != node->m_right) {
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
    else if (node->m_right != successor) {
        successor->m_right->m_parent = successor->m_parent;
        if (successor->m_parent->m_right == successor) {
            successor->m_parent->m_right = successor->m_right;
        }
        else {
            successor->m_parent->m_left = successor->m_right;
        }
    }
    //Switch between successor and current node
    successor->m_parent = node->m_parent;
    if (node->m_parent != nullptr) {
        if (node->m_parent->m_right == node) {
            node->m_parent->m_right = successor;
        }
        else {
            node->m_parent->m_left = successor;
        }
    }
    else {
        m_node = successor;
    }
    successor->m_left = node->m_left;
    if (node->m_left != nullptr) {
        node->m_left->m_parent = successor;
    }
    if (successor != node->m_right) {
        successor->m_right = node->m_right;
        if (node->m_right != nullptr) {
            node->m_right->m_parent = successor;
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