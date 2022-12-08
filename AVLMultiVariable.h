#ifndef AVLMULTIVARIABLE_H
#define AVLMULTIVARIABLE_H

#include "AVLTree.h"
#include "Node.h"
#include "ComplexNode.h"
#include "Exception.h"

template<class T> //does this use the current complexNode?? check by trying something else
class MultiTree : public Tree<ComplexNode<T>, T> {
    template<class M>
    friend class ComplexNode<T>::ComplexNode;
public:
    //ComplexNode<T>* m_root; //Already has m_node from Tree class - what should we do about it?
    
    MultiTree();
    ~MultiTree();
    MultiTree(const MultiTree& other);
    MultiTree& operator=(const MultiTree& other);

    void insert(const T& data, const int id, const int goals, const int cards);
    void print_tree();
        void remove(const int goals, const int cards, const int id);
    T search_and_return_max();
    ComplexNode<T>& search_recursively(const int id, const int goals, const int cards, ComplexNode<T>* currentNode);
    typename ComplexNode<T>::ComplexNode& search_specific_id(const int id, const int goals, const int cards);
    T& search_and_return_data(const int id, const int goals, const int cards);
    ComplexNode<T>* m_node;

};

template<class T>
MultiTree<T>::MultiTree() :
    Tree<ComplexNode<T>, T>()
{}

template<class T>
MultiTree<T>::~MultiTree() {
    this->Tree<ComplexNode<T>, T>::destroy_tree(m_node);
}

template<class T>
void MultiTree<T>::insert(const T& data, const int id, const int goals, const int cards) {
    std::cout << "Height of root: " << m_node->m_height << std::endl;
    //If this is the first node in the tree:
    if (m_node->m_height == -1) {
        m_node->m_data = data;
        m_node->m_id = id;
        m_node->m_height++;
        m_node->m_goals = goals;
        m_node->m_cards = cards;
        return;
    }
    //Find the proper location of the new node (when it's not the first):
    ComplexNode<T>* x = m_node;
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
        node = new ComplexNode<T>;
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
void MultiTree<T>::print_tree() {
    m_node->inorderWalkNode(1);
}

template<class T>
void MultiTree<T>::remove(const int goals, const int cards, const int id) {
    ComplexNode<T>* toRemove = &(search_specific_id(id, goals, cards));
    ComplexNode<T>* nodeToFix = Tree<ComplexNode<T>, T>::make_node_leaf(toRemove);
    delete toRemove;
    //Go up the tree and check the balance factors and complete needed rotations
    Tree<ComplexNode<T>, T>::rebalance_tree(nodeToFix);
}


template<class T>
T MultiTree<T>::search_and_return_max() {
    ComplexNode<T>* node = m_node;
    while(node->m_right != nullptr) {
        node = node->m_right;
    }
    return node->m_data;
}

template<class T>
typename ComplexNode<T>::ComplexNode& MultiTree<T>::search_specific_id(const int id, const int goals, const int cards) {
    return search_recursively(id, goals, cards, m_node);
}


template<class T>
T& MultiTree<T>::search_and_return_data(const int id, const int goals, const int cards) {
    return search_recursively(id, goals, cards, m_node).m_data;
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
    if (currentNode->m_cards > cards) {
        return search_recursively(id, goals, cards, currentNode->m_right);
    }
    if (currentNode->m_id < id) {
        return search_recursively(id, goals, cards, currentNode->m_right);
    }
    return search_recursively(id, goals, cards, currentNode->m_left);
}




#endif //AVLMULTIVARIABLE_H