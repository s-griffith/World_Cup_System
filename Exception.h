#ifndef WORLD_CUP_SYSTEM_EXCEPTION_H
#define WORLD_CUP_SYSTEM_EXCEPTION_H

#include <exception>
#include <iostream>

class NodeNotFound : public std::exception {
    /*
    * Provide information regarding the error - the node that was searched was not found
    * @return - const char* - description of exception
    */
    const char* what() const noexcept override
    {
        return "Node Not Found Error: The following node does not exist in the tree";
    }
};

class InvalidID : public std::exception {
    const char* what() const noexcept override {
        return "Invalid ID Error: ID already exists in the tree.";
    }
};

#endif //WORLD_CUP_SYSTEM_EXCEPTION_H
