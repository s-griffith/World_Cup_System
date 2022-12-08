#ifndef AVLTREE_h
#define AVLTREE_h

#include <iostream>
#include <string>
#include <new>
#include <memory>
#include "Exception.h"
#include "Node.h"

//AVL tree template class
template <class N, class T>
class Tree {
private:
    template<class M>
    friend class Node<T>::Node; //Not actually sure if this is necessary - check after final implementation
public:
    //The tree's root node
    N* m_node;
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
    virtual void destroy_tree(N* currentNode);

    /*
     * Recursively copy full tree
     * @return - none
     */
    void copy_tree(N* currentNode, N* otherNode);

    /*
     * Insert new node with data, according to the id given
     * @param - New data to insert and the ID of the new node
     * @return - void
     */
    void insert(const T& data, const int id);

    /*
     * Remove node according to the id given
     * @param - The ID of the node that needs to be removed
     * @return - none
     */
    void remove(const int id);
    void rebalance_tree(N* currentNode);

    /*
     * Search for node with specific data, according to the id given
     * @param - The ID of the requested node
     * @return - none
     */
    virtual N& search_specific_id(const int id) const;
    virtual N& search_recursively(const int id, N* currentNode) const;
    virtual T& search_and_return_data(const int id) const;

    /*
    * Make the node a leaf without breaking the sorted tree
    * @return - a pointer to the node from which the tree is no longer balanced
    */
    N* make_node_leaf(N* node);

    /*
    * Add all of the tree keys to the given array
    * @return - none
    */
    void get_all_data(int* const array) const;

    void mergeNodes(N* node);
    void print_tree();


//-----------------------------------------FUNCTIONS FOR TESTING--------------------------

    void inorderWalk(bool flag);
};

//--------------------------------------------Tree Class---------------------------------------------------

//Tree Constructor
template <class N, class T>
Tree<N, T>::Tree()
{
    //Creating empty beginning node
    try {
        m_node = new N;
    }
    catch (const std::bad_alloc& e) {
        delete m_node;
        throw std::bad_alloc();
    }
}


//Tree Destructor
template <class N, class T>
Tree<N, T>::~Tree()
{
    destroy_tree(m_node);
}

//Tree Copy Constructor
template <class N, class T>
Tree<N, T>::Tree(const Tree<N, T>& other)
{
    //Create empty beginning node
    try {
        m_node = new N;
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
template <class N, class T>
typename Tree<N, T>::Tree& Tree<N, T>::operator=(const Tree<N, T>& other)
{
    if (this == &other) {
        return *this;
    }
    //Copy the other tree to a new tree
    Tree<N, T> newTree(other);
    //Create temporary tree variable
    N* tmp = newTree.m_node;
    //Copy the current tree to the temporary tree
    newTree.m_node = m_node;
    //Copy the other tree to the current tree
    m_node = tmp;
    return *this;
    //The temporary tree, pointing to the old current tree will now be destroyed
}


//Destroy tree recursively
template <class N, class T>
void Tree<N, T>::destroy_tree(N* currentNode)
{
    N* tmpNode = currentNode;
    if (tmpNode != nullptr) {
        destroy_tree(currentNode->m_left); //this might cause memory leaks with multitree - only place might need to override the function for real
        destroy_tree(currentNode->m_right); //might need dynamic cast here to Node*, or find a way to override destroy func
        delete tmpNode;
    }
}


//Copy tree recursively
template <class N, class T>
void Tree<N, T>::copy_tree(N* currentNode, N* otherNode)
{
    if (otherNode->m_left != nullptr) {
        //Create empty new node
        try {
            currentNode->m_left = new N;
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
            currentNode->m_right = new N;
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

template<class N, class T>
void Tree<N, T>::insert(const T& data, const int id) {
    //If this is the first node in the tree:
    if (m_node->m_height == -1) {
        m_node->m_data = data;
        m_node->m_id = id;
        m_node->m_height++;
        return;
    }
    //Find the proper location of the new node (when it's not the first):
    N* x = m_node;
    N* y = nullptr;
    while (x != nullptr) {
        y = x;
        if (x->m_id == id) {
            throw InvalidID(); //node with that id already exists - invalid operation
        }
        if (id < x->m_id) {
            x = x->m_left;
        }
        else {
            x = x->m_right;
        }
    }
    //Create the new node and add it to the tree:
    N* node = nullptr;
    try{
        node = new N;
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
    node->m_id = id;
    node->m_height = 0;
    if (id < y->m_id) {
        y->m_left = node;
    }
    else {
        y->m_right = node;
    }
    rebalance_tree((node->m_parent));
}


template<class N, class T>
void Tree<N, T>::print_tree() {
    m_node->inorderWalkNode(1);
}


//Remove node according to the id provided
template <class N, class T>
void Tree<N, T>::remove(const int id)
{
    N* toRemove = &(search_specific_id(id));
    N* nodeToFix = make_node_leaf(toRemove);
    delete toRemove;
    //Go up the tree and check the balance factors and complete needed rotations
    rebalance_tree(nodeToFix);
}


//Re-balance tree after node removal
template <class N, class T>
void Tree<N, T>::rebalance_tree(N* currentNode) {
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
template <class N, class T>
N& Tree<N, T>::search_specific_id(const int id) const
{
    return search_recursively(id, m_node);
}


//Combine??
//Search and return node with specific ID
template <class N, class T>
T& Tree<N, T>::search_and_return_data(const int id) const
{
    return search_recursively(id, m_node).m_data;
}


//Search and return node with specific ID recursively
template <class N, class T>
N& Tree<N, T>::search_recursively(const int id, N* currentNode) const
{
    if (currentNode == nullptr) {
        throw NodeNotFound();
    }
    if (currentNode->m_id == id) {
        return *currentNode;
    }
    if (currentNode->m_id < id) {
        return search_recursively(id,currentNode->m_right);
    }
    return search_recursively(id, currentNode->m_left);
}

//Make the current node a leaf (maintaining sort)
template <class N, class T>
N* Tree<N, T>::make_node_leaf(N* node)
{
    //Node to be deleted is already a leaf
    if (node->m_left == nullptr && node->m_right == nullptr) {
        return node->m_parent;
    }
    //Node to be deleted has one child
    if (node->m_left == nullptr || node->m_right == nullptr) {
        N* tmpChild;
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
    N* successor = node->m_right;
    while (successor->m_left != nullptr) {
        successor = successor->m_left;
    }
    N* parentToReturn;
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

template <class N, class T>
void Tree<N, T>::get_all_data(int* const array) const
{
    if (this != nullptr) {
        m_node->get_data_inorder(array, 0);
    }
}

/*
template <class N, class T>
void Tree<N, T>::inorderWalk(bool flag) {
    m_node->inorderWalkNode(flag);
}
*/
/*template<class N, class T>
void Tree<N, T>::mergeNodes(N* node) {
    N* x = node;
    while (x != nullptr) {
        this->insert(x->m_data, x->m_id);
        if (x->m_right != nullptr) {
            this->insert(x->m_right->m_data, x->m_right->m_id);
        }
        if (x->m_left != nullptr) {
            this->insert(x->m_left->m_data, x->m_left->m_id);
        }
        x = x->
    }
} */

template<class N, class T>
void Tree<N, T>::mergeNodes(N* node) {
    if (node == nullptr) {
        return;
    }
    this->mergeNodes(node->m_right);
    this->insert(node->m_data, node->m_id);
    this->mergeNodes(node->m_left);
}

#endif //AVLTREE_h