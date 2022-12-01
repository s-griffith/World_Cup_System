#ifndef NODE_H
#define NODE_H

/*
* Class Tree::Node
* This class is used to create the separate nodes in the tree
*/
template <class T>
class Node{
private:
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
    typename Node<T>::Node<T>* rr_rotation(Node* node);

    /*
     * Right-Left Rotation
     * @return - none
     */
    typename Node<T>::Node<T>* rl_rotation(Node* node);

    /*
     * Left-Left Rotation
     * @return - none
     */
    typename Node<T>::Node<T>* lr_rotation(Node* node);

    /*
     * Left-Right Rotation
     * @return - none
     */
    typename Node<T>::Node<T>* ll_rotation(Node* node);


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
    template <class Node, class M>
    friend class Tree;
    };

//---------------------------------------------------------------------------------------------------------


template <class T>
void Node<T>::printNode() {
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
void Node<T>::printData() {
    printf("Data = %d\n", m_data);
}

template <class T>
void Node<T>::inorderWalkNode(bool flag) {
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


//--------------------------------------------Node Class---------------------------------------------------

//Node Constructor
template <class T>
Node<T>::Node() : m_parent(nullptr), m_left(nullptr), m_right(nullptr), m_height(-1),
    m_id(0), m_bf(0)
    {}



//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
typename Node<T>::Node<T>* Node<T>::ll_rotation(Node* node)
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
typename Node<T>::Node<T>* Node<T>::rr_rotation(Node* node)
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
typename Node<T>::Node<T>* Node<T>::rl_rotation(Node* node)
{
    Node* tmp = m_right->ll_rotation(node);
    tmp = rr_rotation(tmp);
    return tmp;
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
typename Node<T>::Node<T>* Node<T>::lr_rotation(Node* node)
{
    Node* tmp = m_left->rr_rotation(node);
    tmp = ll_rotation(tmp);
    return tmp;
}


//Update the balance factor of the specific node
template <class T>
void Node<T>::update_bf()
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
void Node<T>::update_height()
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


#endif //NODE_H