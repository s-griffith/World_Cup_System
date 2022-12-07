#ifndef NODE_H
#define NODE_H

/*
* Class Tree::Node
* This class is used to create the separate nodes in the tree
*/
template <class T>
class Node {
public:
    //Constructor
    Node();

    //The copy constructor, the assignment operator and the default destructor
    Node(const Node&) = default;
    Node& operator=(const Node& other) = default;
    virtual ~Node() = default;
    int getID() const;
protected:
    //The data that the tree holds
    T m_data;
    //The height of the subtree of this node
    int m_height;
    //Balance factor of node
    int m_bf;
    //The key by which the tree is sorted
    int m_id;

    //Friend classes
    template <class K>
    friend class TreeExtraPointer;

    template <class S>
    friend class MultiTree;

    template <class N, class M>
    friend class Tree;
};

//--------------------------------------------Constructors---------------------------------------------------

//Node Constructor
template <class T>
Node<T>::Node() :
    m_height(-1),
    m_bf(0),
    m_id(0)
{}

//-----------------------------------------------------------------------------------------------------------

template <class T>
int Node<T>::getID() const {
    return m_id;
}

#endif //NODE_H