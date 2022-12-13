#ifndef AVLMULTIVARIABLE_H
#define AVLMULTIVARIABLE_H

#include "AVLTree.h"
#include "Node.h"
#include "ComplexNode.h"
#include "Exception.h"

/*
* Class MultiTree : Tree
* This class is used to create a tree based on ComplexNode that is sorted by three keys:
*       the number of goals the player scored, the number of cards they received and their player ID
*/
template <class T>
class MultiTree : public Tree<ComplexNode<T>, T> {
public:

    //Constructors
    MultiTree();
    ~MultiTree() = default;
    MultiTree(const MultiTree& other);
    MultiTree& operator=(const MultiTree& other);

    /*
     * Insert new node with data, according to the id, goals, and cards given
     * @param - New data to insert and the ID, goals, and cards of the new node
     * @return - void
     */
    void insert(T data, const int id, const int goals, const int cards);

    /*
     * Remove node according to the id, goals, and cards given
     * @param - The ID, goals, and cards of the node that needs to be removed
     * @return - none
     */
    void remove(const int id, const int goals, const int cards);

    /*
     * Search for max node by going down the right side of the tree
     * @param - none
     * @return - the data of the max node
     */
    T& search_and_return_max();
    
    /*
     * Search for a specific node, according to the id, goals, and cards given
     * @param - The ID, goals, and cards of the requested node
     * @return - the data of the node
     */
    T& search_and_return_data(const int id, const int goals, const int cards);

    /*
     * Search for node with a specific id, according to the id, goals, and cards given
     * @param - The ID, goals, and cards of the requested node
     * @return - the requested node
     */
    ComplexNode<T>& search_specific_id(const int id, const int goals, const int cards);

    /*
     * Recursively search the tree, according to the id, goals, and cards given, starting with the root
     * @param - The ID, goals, and cards of the requested node, and the root of its tree
     * @return - the requested node
     */
    ComplexNode<T>& search_recursively(const int id, const int goals, const int cards, ComplexNode<T>* currentNode);

    //Helper function to print the tree - useful for testing
    void print_tree();
};

//-----------------------------------------Constructors--------------------------

template<class T>
MultiTree<T>::MultiTree() :
    Tree<ComplexNode<T>, T>()
{}


template<class T>
MultiTree<T>::MultiTree(const MultiTree& other) {

}


template<class T>
MultiTree<T>& MultiTree<T>::operator=(const MultiTree& other) {

}


//-----------------------------------------Insert and Remove--------------------------

template<class T>
void MultiTree<T>::insert(T data, const int id, const int goals, const int cards) {
    //If this is the first node in the tree:
    if (this->m_node->m_height == -1) {
        this->m_node->m_data = data;
        this->m_node->m_id = id;
        this->m_node->m_height++;
        this->m_node->m_goals = goals;
        this->m_node->m_cards = cards;
        return;
    }
    //Find the proper location of the new node (when it's not the first):
    ComplexNode<T>* x = this->m_node;
    ComplexNode<T>* parent = nullptr;
    while (x != nullptr) {
        parent = x;
        if (x->m_id == id) {
            throw InvalidID(); //node with that id already exists - invalid operation
        }
        //maybe make this into a switch case
        if (goals < x->m_goals) {
            x = x->m_left;
        }
        else if (goals == x->m_goals) {
            if (cards > x->m_cards) {
                x = x->m_left;
            }
            else if (cards == x->m_cards) {
                if (id < x->m_id) {
                    x = x->m_left;
                }
                else {
                    x = x->m_right;
                }
            }
            else {
                x = x->m_right;
            }
        }
        else {
            x = x->m_right;
        }
    }
    //Create the new node and add it to the tree:
    ComplexNode<T>* node = nullptr;
    try{
        node = new ComplexNode<T>();
    }
    catch(const std::bad_alloc& e)
    {
        delete node;
        throw e;
    }
    node->m_parent = parent;
    node->m_left = nullptr;
    node->m_right = nullptr;
    node->m_data = data;
    node->m_goals = goals;
    node->m_cards = cards;
    node->m_id = id;
    node->m_height = 0;
    if (node->m_goals < parent->m_goals) {
        parent->m_left = node;
    }
    else if (node->m_goals == parent->m_goals) {
        if (node->m_cards > parent->m_cards) {
            parent->m_left = node;
        }
        else if (node->m_cards == parent->m_cards) {
            if (node->m_id < parent->m_id) {
                parent->m_left = node;
            }
            else {
                parent->m_right = node;
            }
        }
        else {
            parent->m_right = node;
        }
    }
    else {
        parent->m_right = node;
    }
    this->rebalance_tree(node->m_parent);
}


template<class T>
void MultiTree<T>::remove(const int id, const int goals, const int cards) {
    ComplexNode<T>* toRemove = &(search_specific_id(id, goals, cards));
    ComplexNode<T>* nodeToFix = Tree<ComplexNode<T>, T>::make_node_leaf(toRemove);
    delete toRemove;
    //Go up the tree and check the balance factors and complete needed rotations
    Tree<ComplexNode<T>, T>::rebalance_tree(nodeToFix);
}


//-----------------------------------------Search Functions--------------------------

template<class T>
T& MultiTree<T>::search_and_return_max() { //if this makes problems, get rid of reference at return
    ComplexNode<T>* node = this->m_node;
    while(node->m_right != nullptr) {
        node = node->m_right;
    }
    return node->m_data;
}


template<class T>
T& MultiTree<T>::search_and_return_data(const int id, const int goals, const int cards) {
    return search_recursively(id, goals, cards, this->m_node).m_data;
}


template<class T>
ComplexNode<T>& MultiTree<T>::search_specific_id(const int id, const int goals, const int cards) {
    return search_recursively(id, goals, cards, this->m_node);
}


template<class T>
ComplexNode<T>& MultiTree<T>::search_recursively(const int id, const int goals, const int cards,
             ComplexNode<T>* currentNode) {
    if (currentNode == nullptr) {
        throw NodeNotFound();
    }
    if (currentNode->m_id == id) {
        return *currentNode;
    }
    if (currentNode->m_goals < goals) {
        return search_recursively(id, goals, cards, currentNode->m_right);
    }
    if (currentNode->m_cards > cards && currentNode->m_goals == goals) {
        return search_recursively(id, goals, cards, currentNode->m_right);
    }
    if (currentNode->m_id < id && currentNode->m_goals == goals && currentNode->m_cards == cards) {
        return search_recursively(id, goals, cards, currentNode->m_right);
    }
    return search_recursively(id, goals, cards, currentNode->m_left);
}


//-----------------------------------------Helper Function for Testing--------------------------

template<class T>
void MultiTree<T>::print_tree() {
    this->m_node->inorderWalkNode(1);
}

//----------------------------------------------------------------------------------------------

#endif //AVLMULTIVARIABLE_H