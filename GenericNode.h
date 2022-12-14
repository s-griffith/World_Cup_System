#ifndef WORLD_CUP_SYSTEM_GENERICNODE_H
#define WORLD_CUP_SYSTEM_GENERICNODE_H
#include "Node.h"
#include "Teams.h"

/*
* Class Tree : Generic Node
* This class is used to create the separate nodes in the tree - of the general kind (no extra keys or pointers)
*/
template <class T>
class GenericNode : Node<T> {
public:

    //Constructor
    GenericNode();

    //The copy constructor, the assignment operator and the default destructor------------------------need to implement these!!!
    GenericNode(const GenericNode&) = default;
    GenericNode& operator=(const GenericNode& other) = default;
    ~GenericNode() = default;

    /*
     * Helper function for knockout in world_cup:
     * Find the number of valid teams within a given range
     * @param - The minimum and maximum team ID's
     * @return - the number of teams found
     */
    int numOfTeams(const int minTeamID, const int maxTeamID);

    /*
     * Helper function for knockout in world_cup:
     * Adds valid teams within a given range to a given array
     * @param - The array to add the teams to and the minimum and maximum team ID's
     * @return - none
     */
    void addTeams(Team* teams, const int minTeamId, const int maxTeamId);
    void inorderWalkTeamID(Team* team);

    int get_height() const;

protected:

    //Pointers to the parent node and the two child nodes
    GenericNode* m_parent;
    GenericNode* m_left;
    GenericNode* m_right;

    /*
     * Left-Right Rotation
     * @return - none
     */
    GenericNode* ll_rotation(GenericNode* node);

    /*
     * Right-Right Rotation
     * @return - none
     */
    GenericNode* rr_rotation(GenericNode* node);

    /*
     * Right-Left Rotation
     * @return - none
     */
    GenericNode* rl_rotation(GenericNode* node);

    /*
     * Left-Left Rotation
     * @return - none
     */
    GenericNode* lr_rotation(GenericNode* node);

    /*
     * Update balance factor of the current node
     * @return - none
    */
    void update_bf();
    
    /*
     * Update height of the current node
     * @return - none
    */
    void update_height();

    /*
     * Helper function for get_all_players in world_cup:
     * Recursively inserts all players in a tree into a given array, starting with the root of the tree
     * @return - none
    */
    int get_data_inorder(int* const array, int index) const;

    /*
     * Helper function for knockout in world_cup:
     * Recursively counts the number of valid teams within a given range. Called by numOfTeams().
     * @param - A counter and the maximum and minimum ID's
     * @return - number of teams found
     */
    int inorderWalkCount(int counter, const int minID, const int maxID) const;
        
    /*
     * Helper function for knockout in world_cup:
     * Recursively inserts valid teams into the array given
     * @param - The array to add the teams to and the minimum and maximum team ID's
     * @return - none
     */
    void inorderWalkInsert(Team* teams, const int minID, const int maxID);//, int index);

    /*
     * Helper function for knockout in world_cup:
     * Finds the valid team with the lowest team ID within a given range to a given array
     * @param - The minimum and maximum team ID's
     * @return - a pointer to the team found
     */
    typename GenericNode<T>::GenericNode* getFirstTeam(const int minTeamId, const int maxTeamId);

    //Helper functions to print
    void inorderWalkNode(bool flag);
    void printNode();
    void printData();
    
    //Friend classes
    template <class N, class M>
    friend class Tree;
};

//--------------------------------------------Constructors---------------------------------------------------

template <class T>
GenericNode<T>::GenericNode() :
        Node<T>(),
        m_parent(nullptr),
        m_left(nullptr),
        m_right(nullptr)
{}


//--------------------------------Public Helper Functions for world_cup--------------------------------------------

template<class T>
int GenericNode<T>::numOfTeams(const int minTeamID, const int maxTeamID) {
    GenericNode<T>* first = this->getFirstTeam(minTeamID, maxTeamID);
    //std::cout << "First is " << first->getID() << std::endl;
    if (first == nullptr) {
        return 0;
    }
    return (first->m_data->knockout_count(maxTeamID));
    //return (first->inorderWalkCount(0, minTeamID, maxTeamID));
}


template<class T>
void GenericNode<T>::addTeams(Team* teams, const int minTeamId, const int maxTeamId) {
    GenericNode<T>* first = this->getFirstTeam(minTeamId, maxTeamId);
    first->m_data->knockout_insert(teams, maxTeamId);
    //first->inorderWalkInsert(teams, minTeamId, maxTeamId);
}


template<class T>
int GenericNode<T>::get_height() const {
    return this->m_height;
}


//--------------------------------------------Rotations---------------------------------------------------

//Left-Left tree rotation, on the node with balance factor of +2
template <class T>
typename GenericNode<T>::GenericNode* GenericNode<T>::ll_rotation(GenericNode<T>* node)
{
    GenericNode<T>* tmpToReturn = node;
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
typename GenericNode<T>::GenericNode* GenericNode<T>::rr_rotation(GenericNode<T>* node)
{
    GenericNode<T>* tmpToReturn = node;
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
typename GenericNode<T>::GenericNode* GenericNode<T>::rl_rotation(GenericNode<T>* node)
{
    GenericNode<T>* tmp = m_right->ll_rotation(node);
    tmp = rr_rotation(tmp);
    return tmp;
}

//Left-Right tree rotation, on the node with balance factor of +2
template <class T>
typename GenericNode<T>::GenericNode* GenericNode<T>::lr_rotation(GenericNode<T>* node)
{
    GenericNode<T>* tmp = m_left->rr_rotation(node);
    tmp = ll_rotation(tmp);
    return tmp;
}

//----------------------------------------------Node Stats---------------------------------------------------

//Update the balance factor of the specific node
template <class T>
void GenericNode<T>::update_bf()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    Node<T>::m_bf = heightLeft - heightRight;
}


//Update the subtree height of the specific node
template <class T>
void GenericNode<T>::update_height()
{
    int heightLeft = 0, heightRight = 0;
    if (m_left != nullptr) {
        heightLeft = m_left->m_height + 1;
    }
    if (m_right != nullptr) {
        heightRight = m_right->m_height + 1;
    }
    if (heightLeft >= heightRight) {
        Node<T>::m_height = heightLeft;
    }
    else {
        Node<T>::m_height = heightRight;
    }
}


//---------------------------------Private Helper Functions for world_cup---------------------------------------------

//Need to delete
template <class T>
int GenericNode<T>::get_data_inorder(int* const array, int index) const
{
    if (this != nullptr) {
        index = m_left->get_data_inorder(array, index);
        array[index++] = this->m_id;
        index = m_right->get_data_inorder(array, index);
    }
    return index;
}


template <class T>
int GenericNode<T>::inorderWalkCount(int counter, const int minID, const int maxID) const { //This isn't catching all examples - check this
    if (this != nullptr && this->m_id <= maxID && this->m_id >= minID) {
        if (this->m_id == maxID) {
            return 1;
        }
        if (m_parent != nullptr && m_parent->m_right == this) {
            return counter += 1 + m_parent->m_parent->inorderWalkCount(counter, minID, maxID);
        }
        else if (m_parent != nullptr && m_parent->m_left == this) {
            return counter += 1 + m_parent->inorderWalkCount(counter, minID, maxID);
        }
        return counter += 1 + m_right->inorderWalkCount(counter, minID, maxID);
    }
   // else if (this->m_id > maxID) {
    //    return counter += m_left->inorderWalkCount(counter, minID, maxID); //Creates infinite loop
    //}
    return counter;






}


template <class T>
void GenericNode<T>::inorderWalkInsert(Team* teams, const int minID, const int maxID){//, int index) {

  /*  if (this != nullptr && this->m_id <= maxID && this->m_id >= minID) {
        if (this->m_id == maxID) {
            *(teams+index) = *(this->m_data);
            return;
        }
        if (m_parent != nullptr && m_parent->m_right == this) {
            m_parent->m_parent->inorderWalkInsert(teams, minID, maxID, index+1);
        }
        else if (m_parent != nullptr && m_parent->m_left == this) {
            m_parent->inorderWalkInsert(teams, minID, maxID, index+1);
        }
        m_right->inorderWalkInsert(teams, minID, maxID, index+1);
        *(teams+index) = *(this->m_data);
    } */
}


template<class T>
typename GenericNode<T>::GenericNode* GenericNode<T>::getFirstTeam(const int minTeamId, const int maxTeamId){
    GenericNode<Team*>* x = this;
    GenericNode<Team*>* y = nullptr;
    while (x != nullptr) {
        if (x->m_id > minTeamId) {
            if (x->m_left != nullptr) {
                if (x->m_left->m_id < minTeamId) {
                    return x;
                }
            }
        }
        y = x;
        if (x->m_id == minTeamId) {
            return x; //node with that id already exists - invalid operation
        }
        if (minTeamId < x->m_id) {
            x = x->m_left;
        }
        else {
            x = x->m_right;
        }
    }
    return y;
}


//------------------------------------Printing Functions for Testing-------------------------------------------------

template <class T>
void GenericNode<T>::printNode() {
    int parent, left, right;
    if (m_parent == nullptr) {
        parent = -1;
    }
    else {
        parent = m_parent->m_id;
    }
    if (m_left == nullptr) {
        left = -1;
    }
    else {
        left = m_left->m_id;
    }
    if (m_right == nullptr) {
        right = -1;
    }
    else {
        right = m_right->m_id;
    }
    std::cout << "ID = " << Node<T>::m_id << ", Parent = " << parent << ", Left = " << left << ", Right = " << right << std::endl;
}


template <class T>
void GenericNode<T>::printData() {
    std::cout << "Data = " << this->m_data << std::endl;
}


template <class T>
void GenericNode<T>::inorderWalkNode(bool flag) {
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

template <class T>
void GenericNode<T>::inorderWalkTeamID(Team* team) {
    if (this != nullptr) {
        m_left->inorderWalkTeamID(team);
        this->m_data->update_team(team);
        m_right->inorderWalkTeamID(team);
    }
}

//-----------------------------------------------------------------------------------------------------------

#endif //WORLD_CUP_SYSTEM_GENERICNODE_H
