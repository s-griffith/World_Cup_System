#ifndef WORLD_CUP_SYSTEM_AVLTREEBYSCORE_H
#define WORLD_CUP_SYSTEM_AVLTREEBYSCORE_H

#include "AVLTree.h"
#include "Node.h"
#include "NodeAdditionalPointer.h"

//AVL tree special class, derived from the regular AVL tree class
//This class has an extra data pointer in each node, pointing to
template <class T>
class TreeByScore : public Tree<NodeWithPointer<T>, T> {
public:
    //Tree Constructor
    TreeByScore();

    //Tree Copy constructor, assignment operator and destructor - use the AVLTree's functions
    ~TreeByScore() = default;
    TreeByScore(const TreeByScore<T>& other) = default;
    TreeByScore& operator=(const TreeByScore<T>& other) = default;

    /*
    * Search for the node with the given id, and then update the pointer to the  matching node in another tree
    * @param - The ID of the node that needs to be updated, and a pointer to the node in the other tree
    * @return - none
    */
    void update_additional_pointer(const int id, typename Node<T>::Node* otherNode);

    /*
    * Search for the node with the given id, access the matching node in the other tree, and return the closest
    *       node in the other tree
    * @param - The ID of the node that needs to be searched for
    * @return - The data of the closest node in the other tree
    */
    T& get_closest(const int id);
    typename Node<T>::Node* findLeftClosest(typename Node<T>::Node* otherTreeNode);
    typename Node<T>::Node* findRightClosest(typename Node<T>::Node* otherTreeNode);
};

//--------------------------------------------Tree Class---------------------------------------------------

//Tree Constructor
template <class T>
TreeByScore<T>::TreeByScore() : Tree<NodeWithPointer<T>, T>()
{}


template <class T>
void TreeByScore<T>::update_additional_pointer(const int id, typename Node<T>::Node* otherNode)
{
    typename Node<T>::Node* tmpNode = Tree<NodeWithPointer<T>, T>::search_specific_id(id);
    tmpNode->m_matchOtherTree = otherNode;
}


template <class T>
T& TreeByScore<T>::get_closest(const int id)
{
    //Search for specific node
    typename Node<T>::Node* thisTreeNode = Tree<NodeWithPointer<T>, T>::search_specific_id(id);
    //Access the matching node in the other tree
    typename Node<T>::Node* otherTreeNode = thisTreeNode->m_matchOtherTree;
    //Get closest node to the left of the other tree node
    typename Node<T>::Node* closestLeft = findLeftClosest(otherTreeNode);
    //Get closest node to the right of the other tree node
    typename Node<T>::Node* closestRight = findRightClosest(otherTreeNode);
    //Get closest node overall
    int tmpId = (otherTreeNode->m_data).get_closest(closestLeft->m_data, closestRight->m_data);
    if (tmpId == closestLeft->m_id) {
        return closestLeft->m_data;
    }
    return closestRight->m_data;
}


template <class T>
typename Node<T>::Node* TreeByScore<T>::findLeftClosest(typename Node<T>::Node* otherTreeNode)
{
    typename Node<T>::Node* closestLeft = otherTreeNode;
    if (closestLeft->m_left != nullptr) {
        closestLeft = closestLeft->m_left;
        while (closestLeft->m_right != nullptr) {
            closestLeft = closestLeft->m_right;
        }
    }
    if ((otherTreeNode->m_parent != nullptr) && (otherTreeNode->m_parent->m_right == otherTreeNode)) {
        if (closestLeft->m_id == otherTreeNode->m_id) {
            closestLeft = otherTreeNode->m_parent;
        }
        else {
            //Check which node is closer: the child to the left or the parent
            int tmpId = (otherTreeNode->m_data).is_closer(closestLeft->m_data, otherTreeNode->m_parent->m_data);
            if (tmpId == otherTreeNode->m_parent->m_id) {
                closestLeft = otherTreeNode->m_parent;
            }
        }
    }
    if (closestLeft->m_id != otherTreeNode->m_id) {
        return closestLeft;
    }
    return nullptr;
}


template <class T>
typename Node<T>::Node* TreeByScore<T>::findRightClosest(typename Node<T>::Node* otherTreeNode)
{
    typename Node<T>::Node* closestRight = otherTreeNode;
    if (closestRight->m_right != nullptr) {
        closestRight = closestRight->m_right;
        while (closestRight->m_left != nullptr) {
            closestRight = closestRight->m_left;
        }
    }
    if ((otherTreeNode->m_parent != nullptr) && (otherTreeNode->m_parent->m_left == otherTreeNode)) {
        if (closestRight->m_id == otherTreeNode->m_id) {
            closestRight = otherTreeNode->m_parent;
        }
        else {
            //Check which node is closer: the child to the right or the parent
            int tmpId = (otherTreeNode->m_data).get_closest(closestRight->m_data, otherTreeNode->m_parent->m_data);
            if (tmpId == otherTreeNode->m_parent->m_id) {
                closestRight = otherTreeNode->m_parent;
            }
        }
    }
    if (closestRight->m_id != otherTreeNode->m_id) {
        return closestRight;
    }
    return nullptr;
}

#endif //WORLD_CUP_SYSTEM_AVLTREEBYSCORE_H