#ifndef AVLTREE_h
#define AVLTREE_h

#include <iostream>
#include <string>
#include <new>

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

    /*
     * Search for node with specific data, according to the id given
     * @return - none
     */
    Node& search_specific_id(const int id) const;

    /*
     * Search for node with specific data, according to the id given
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
void Tree<T>::destroy_tree(Node* m_node)
{
    Node* currentNode = m_node;
    if (currentNode != nullptr) {
        destroyTree(m_node->m_left);
        destroyTree(m_node->m_right);
        delete currentNode;
    }
}

//Copy tree recursively-----------------------------------------------------------------UNFINISHED
template <class T>
void Tree<T>::copy_tree(Node* m_node, const Tree& other)
{
    if (other.get_left() != nullptr) {
        //Create empty new node
        try {
            m_node->m_left = new Node;
            m_node->m_left->m_data = other.get_left_data();
            copy_tree(m_node->m_left, other.get_left())
        }
        catch (const std::bad_alloc& e) {
            delete m_node->m_left;
            throw std::bad_alloc();
        }
    }
    if (other->get_right() != nullptr) {
        //Create empty new node
        try {
            m_node->m_right = new Node;
            m_node->m_right->m_data = other.get_right_data();
            copy_tree(m_node->m_right, other.get_right())
        }
        catch (const std::bad_alloc& e) {
            delete m_node->m_right;
            throw std::bad_alloc();
        }
    }
}

//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
void Tree<T>::ll_rotation()
{
    //Changing A->B to A->Parent
    m_node->m_left->m_parent = m_node->m_parent;
    //Changing Parent->B to Parent->A
    if (m_node->m_parent != nullptr) {
        if (m_node->m_parent->m_left == m_node) {
            m_node->m_parent->m_left = m_node->m_left;
        }
        else {
            m_node->m_parent->m_right = m_node->m_left;
        }
    }
    //Changing B->Parent to B->A
    m_node->m_parent = m_node->m_left;
    //Changing Ar->A to Ar->B
    if (m_node->m_left->m_right != nullptr) {
        m_node->m_left->m_right->m_parent = m_node;
    }
    //Changing B->A to B->Ar
    m_node->m_left = m_node->m_left->m_right;
    //Changing A->Ar to A->B
    m_node->m_parent->m_right = m_node;
}

//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
void Tree<T>::lr_rotation()
{
    //Changing B->A to B->Parent
    m_node->m_left->m_right->m_parent = m_node->m_parent;
    //Changing Parent->C to Parent->B
    if (m_node->m_parent != nullptr) {
        if (m_node->m_parent->m_left == m_node) {
            m_node->m_parent->m_left = m_node->m_left->m_right;
        }
        else {
            m_node->m_parent->m_right = m_node->m_left->m_right;
        }
    }
    //Changing C->Parent to C->B
    m_node->m_parent = m_node->m_left->m_right;
    //Changing A->B to A->Bl
    m_node->m_left->m_right = m_node->m_left->m_right->m_left;
    //Changing Bl->B to Bl->A
    m_node->m_parent->m_left->m_parent = m_node->m_left;
    //Changing B->Bl to B->A
    m_node->m_parent->m_left = m_node->m_left;
    //Changing A->C to A->B
    m_node->m_left->m_parent = m_node->m_parent;
    //Changing C->A to C->Br
    m_node->m_left = m_node->m_parent->m_right;
    //Changing Br->B to Br->C
    if (m_node->m_parent->m_right != nullptr) {
        m_node->m_parent->m_right->m_parent = m_node;
    }
    //Changing B->Br to B->C
    m_node->m_parent->m_right = m_node;
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

//---------------------------------------------------------------------------------------------------------


#endif //AVLTREE_h