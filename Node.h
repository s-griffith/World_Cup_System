#ifndef NODE_H
#define NODE_H


/*
* Class Tree::Node
* This class is used to create the separate nodes in the tree
*/
template <class T>
class Node{
public:
    //Constructor
    Node();
    /*
     * The copy constructor, the assignment operator and the default destructor
    */
    Node(const Node&) = default;
    Node& operator=(const Node& other) = default;
    virtual ~Node() = default;

protected:
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

    /*
     * Right-Right Rotation
     * @return - none
     */
    Node* rr_rotation(Node* node);

    /*
     * Right-Left Rotation
     * @return - none
     */
    Node* rl_rotation(Node* node);

    /*
     * Left-Left Rotation
     * @return - none
     */
    Node* lr_rotation(Node* node);

    /*
     * Left-Right Rotation
     * @return - none
     */
    Node* ll_rotation(Node* node);


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

    void inorderWalkNode(bool flag);
    virtual void printNode();
    virtual void printData();

    template <class N, class M>
    friend class Tree;

    template <class K>
    friend class TreeExtraPointer;
    };

//---------------------------------------------------------------------------------------------------------

/*
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
    std::cout << "Data = " << m_data << ", Parent = " << parent << ", Left = " << left << ", Right = " << right << std::endl;
}

template <class T>
void Node<T>::printData() {
    std::cout << "Data = " << m_data << std::endl;
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
*/
//--------------------------------------------Constructors---------------------------------------------------

//Node Constructor
template <class T>
Node<T>::Node() : m_parent(nullptr), m_left(nullptr), m_right(nullptr), m_height(-1),
    m_id(0), m_bf(0)
    {}


//--------------------------------------------Rotations---------------------------------------------------


//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
typename Node<T>::Node* Node<T>::ll_rotation(Node<T>* node)
{
    Node<T>* tmpToReturn = node;
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
typename Node<T>::Node* Node<T>::rr_rotation(Node<T>* node)
{
    Node<T>* tmpToReturn = node;
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
typename Node<T>::Node* Node<T>::rl_rotation(Node<T>* node)
{
    Node<T>* tmp = m_right->ll_rotation(node);
    tmp = rr_rotation(tmp);
    return tmp;
}


//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
typename Node<T>::Node* Node<T>::lr_rotation(Node<T>* node)
{
    Node<T>* tmp = m_left->rr_rotation(node);
    tmp = ll_rotation(tmp);
    return tmp;
}

//--------------------------------------------Node Stats---------------------------------------------------


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

template <class T>
void Node<T>::get_data_inorder(int* const array, int index) const
{
    if (m_left != nullptr) {
        m_left->get_data_inorder(array, index);
    }
    array[index++] = m_id;
    if (m_right != nullptr) {
        m_right->get_data_inorder(array, index);
    }
}

//---------------------------------------------------------------------------------------------------------


#endif //NODE_H