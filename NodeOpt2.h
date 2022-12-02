#ifndef NODE_H
#define NODE_H

/*
* Class Tree::Node
* This class is used to create the separate nodes in the tree
*/
template <class N, class T>
class Node{
public:
    //Constructor
    Node();
    /*
     * The copy constructor, the assignment operator and the default destructor
    */
    Node(const Node&);
    Node& operator=(const Node& other);
    virtual ~Node() = default;

private:
    //Pointers to the parent node and the two child nodes
    N* m_parent;
    N* m_left;
    N* m_right;
    //The data that the tree holds
    T m_data;
    //The height of the subtree of this node
    int m_height;
    //Balance factor of node
    int m_bf;
    //The key by which the tree is sorted
    int m_id;

    /*
     * Right-Right Rotation
     * @return - none
     */
    N* rr_rotation(N* node, N* it);

    /*
     * Right-Left Rotation
     * @return - none
     */
    N* rl_rotation(N* node, N* it);

    /*
     * Left-Left Rotation
     * @return - none
     */
    N* lr_rotation(N* node, N* it);

    /*
     * Left-Right Rotation
     * @return - none
     */
    N* ll_rotation(N* node, N* it);


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
    virtual void printNode();
    virtual void printData();

    template <class Node, class M>
    friend class Tree;
    };

//---------------------------------------------------------------------------------------------------------


template <class N, class T>
void Node<N, T>::printNode() {
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
    std::cout << "Data = " << m_data << ", Parent = " << parent << ", Left = " << left << ", Right = " << right << std::endl;
}

template <class N, class T>
void Node<N, T>::printData() {
    std::cout << "Data = " << m_data << std::endl;
}

template <class N, class T>
void Node<N, T>::inorderWalkNode(bool flag) {
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


//--------------------------------------------Constructors---------------------------------------------------

//Node Constructor
template <class N, class T>
Node<N, T>::Node() : m_parent(nullptr), m_left(nullptr), m_right(nullptr), m_height(-1),
    m_id(0), m_bf(0)
    {}


//--------------------------------------------Rotations---------------------------------------------------


//Left-Left tree rotation, on the node with balance factor of +2
template <class N, class T>
N* Node<N, T>::ll_rotation(N* node, N* it) //it is this but with the proper type and not just general Node*
{
    N* tmpToReturn = node;
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
        m_left->m_right->m_parent = it;
    }
    //Changing B->A to B->Ar
    m_left = m_left->m_right;
    //Changing A->Ar to A->B
    m_parent->m_right = it;
    return tmpToReturn;
}

//Right-Right tree rotation, on the node with balance factor of -2
template <class N, class T>
N* Node<N, T>::rr_rotation(N* node, N* it)
{
    N* tmpToReturn = node;
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
        m_right->m_left->m_parent = it;
    }
    m_right = m_right->m_left;
    m_parent->m_left = it;
    return tmpToReturn;
}


//Right-Left tree rotation, on the node with balance factor of -2
template <class N, class T>
N* Node<N, T>::rl_rotation(N* node, N* it)
{
    N* tmp = m_right->ll_rotation(node, it);
    tmp = rr_rotation(tmp, it);
    return tmp;
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class N, class T>
N* Node<N, T>::lr_rotation(N* node, N* it)
{
    N* tmp = m_left->rr_rotation(node, it);
    tmp = ll_rotation(tmp, it);
    return tmp;
}

//--------------------------------------------Node Stats---------------------------------------------------


//Update the balance factor of the specific node
template <class N, class T>
void Node<N, T>::update_bf()
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
template <class N, class T>
void Node<N, T>::update_height()
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