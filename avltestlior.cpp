#include "AVLTree.h"
#include "Node.h"

int main() {
    Tree<Node<int>, int> t1;
    t1.insert(5,5);
    printf("One node with '5': \n");
    t1.inorderWalk(1);
    printf("\n");

    t1.insert(20,20);
    t1.insert(-14,-14);
    t1.insert(50,50);
    t1.insert(2,2);
    t1.insert(3,3);
    t1.insert(4,4);


    printf("Original Tree:\n");
    t1.inorderWalk(1);
    printf("\n");



    t1.remove(20);
    printf("Removal of 20:\n");
    t1.inorderWalk(1);
    printf("\n");


    t1.remove(2);
    printf("Removal of 2:\n");
    t1.inorderWalk(1);
    printf("\n");

    try {
        t1.remove(70);
    }
    catch (...) {
        printf("Couldn't remove 70!\n");
    }

    int i = t1.insert(5,5);
    printf("Added 5 again: %d\n",i);

    t1.remove(3);
    printf("Removed 3:\n");
    t1.inorderWalk(1);
    printf("\n");

    Tree<Node<int>, int> t2 = t1;
    printf("Print 'operation=' tree:\n");
    t2.inorderWalk(1);
    printf("\n");


    Tree<Node<int>, int> t3(t1);
    printf("Print copied tree:\n");
    t3.inorderWalk(1);
    printf("\n");

};

