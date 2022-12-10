#ifndef WORLD_CUP_SYSTEM_AVLTREEEXTRAPOINTER_H
#define WORLD_CUP_SYSTEM_AVLTREEEXTRAPOINTER_H

#include "AVLTree.h"
#include "Node.h"
#include "ComplexNode.h"
#include "NodeAdditionalPointer.h"

//AVL tree special class, derived from the regular AVL tree class
//This class has an extra data pointer in each node, pointing to
template <class T>
class TreeExtraPointer : public Tree<NodeWithPointer<T>, T> {
protected:
    template<class K>
    friend
    class NodeWithPointer; //Not actually sure if this is necessary - check after final implementation

public:
    //Tree Constructor
    TreeExtraPointer();

    //Tree Copy constructor, assignment operator and destructor - use the AVLTree's functions
    ~TreeExtraPointer() = default;

    TreeExtraPointer(const TreeExtraPointer<T> &other) = default;

    TreeExtraPointer &operator=(const TreeExtraPointer<T> &other) = default;

    /*
    * Search for the node with the given id, and then update the pointer to the  matching node in another tree
    * @param - The ID of the node that needs to be updated, and a pointer to the node in the other tree
    * @return - none
    */
    void update_additional_pointer(const int id, typename ComplexNode<T>::ComplexNode *otherNode);

    /*
    * Search for the node with the given id, access the matching node in the other tree, and return the closest
    *       node in the other tree
    * @param - The ID of the node that needs to be searched for
    * @return - The data of the closest node in the other tree
    */
    int get_closest(const int id);

    typename ComplexNode<T>::ComplexNode *findLeftClosest(typename ComplexNode<T>::ComplexNode *otherTreeNode);

    typename ComplexNode<T>::ComplexNode *findRightClosest(typename ComplexNode<T>::ComplexNode *otherTreeNode);

    void mergeNodesExtraPointer(typename NodeWithPointer<T>::NodeWithPointer *node);
};

//--------------------------------------------Tree Class---------------------------------------------------

//Tree Constructor
template <class T>
TreeExtraPointer<T>::TreeExtraPointer() : Tree<NodeWithPointer<T>, T>()
{}


template <class T>
void TreeExtraPointer<T>::update_additional_pointer(const int id, typename ComplexNode<T>::ComplexNode* otherNode)
{
    typename NodeWithPointer<T>::NodeWithPointer* tmpNode = &(Tree<NodeWithPointer<T>, T>::search_specific_id(id));
    tmpNode->m_matchOtherTree = otherNode;
}


template <class T>
int TreeExtraPointer<T>::get_closest(const int id)
{
    //Search for specific node
    typename NodeWithPointer<T>::NodeWithPointer* thisTreeNode = &(Tree<NodeWithPointer<T>, T>::search_specific_id(id));
    //Get closest node to the left of the other tree node
    typename ComplexNode<T>::ComplexNode* closestLeft = findLeftClosest(thisTreeNode->m_matchOtherTree);
    //Get closest node to the right of the other tree node
    typename ComplexNode<T>::ComplexNode* closestRight = findRightClosest(thisTreeNode->m_matchOtherTree);
    //Get closest node overall
    if (closestRight == nullptr) {
        return closestLeft->m_id;
    }
    if (closestLeft == nullptr) {
        return closestRight->m_id;
    }
    int tmpId = thisTreeNode->m_data->get_closest(closestLeft->m_data, closestRight->m_data);
    return tmpId;
}


template <class T>
typename ComplexNode<T>::ComplexNode* TreeExtraPointer<T>::findLeftClosest(typename ComplexNode<T>::ComplexNode* currentPlayerNode)
{
    typename ComplexNode<T>::ComplexNode* closestLeft = currentPlayerNode;
    if (closestLeft->m_left != nullptr) {
        closestLeft = closestLeft->m_left;
        while (closestLeft->m_right != nullptr) {
            closestLeft = closestLeft->m_right;
        }
    }
    if ((currentPlayerNode->m_parent != nullptr) && (currentPlayerNode->m_parent->m_right == currentPlayerNode)) {
        if (closestLeft->m_id == currentPlayerNode->m_id) {
            closestLeft = currentPlayerNode->m_parent;
        }
        else {
            //Check which node is closer: the child to the left or the parent
            int tmpId = currentPlayerNode->m_data->get_closest(closestLeft->m_data, currentPlayerNode->m_parent->m_data);
            if (tmpId == currentPlayerNode->m_parent->m_id) {
                closestLeft = currentPlayerNode->m_parent;
            }
        }
    }
    if (closestLeft->m_id != currentPlayerNode->m_id) {
        return closestLeft;
    }
    return nullptr;
}


template <class T>
typename ComplexNode<T>::ComplexNode* TreeExtraPointer<T>::findRightClosest(typename ComplexNode<T>::ComplexNode* currentPlayerNode)
{
    typename ComplexNode<T>::ComplexNode* closestRight = currentPlayerNode;
    if (closestRight->m_right != nullptr) {
        closestRight = closestRight->m_right;
        while (closestRight->m_left != nullptr) {
            closestRight = closestRight->m_left;
        }
    }
    if ((currentPlayerNode->m_parent != nullptr) && (currentPlayerNode->m_parent->m_left == currentPlayerNode)) {
        if (closestRight->m_id == currentPlayerNode->m_id) {
            closestRight = currentPlayerNode->m_parent;
        }
        else {
            //Check which node is closer: the child to the right or the parent
            int tmpId = currentPlayerNode->m_data->get_closest(closestRight->m_data, currentPlayerNode->m_parent->m_data);
            if (tmpId == currentPlayerNode->m_parent->m_id) {
                closestRight = currentPlayerNode->m_parent;
            }
        }
    }
    if (closestRight->m_id != currentPlayerNode->m_id) {
        return closestRight;
    }
    return nullptr;
}

template <class T>
void TreeExtraPointer<T>::mergeNodesExtraPointer(typename NodeWithPointer<T>::NodeWithPointer* node) {
    if (node == nullptr) {
        return;
    }
    this->mergeNodesExtraPointer(node->m_right);
    try {
        this->insert(node->m_data, node->m_id);
        this->update_additional_pointer(node->m_id, node->m_matchOtherTree);
    }
    catch (const InvalidID& e) {}
    this->mergeNodesExtraPointer(node->m_left);
}

#endif //WORLD_CUP_SYSTEM_AVLTREEEXTRAPOINTER_H