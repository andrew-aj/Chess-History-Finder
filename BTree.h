#pragma once

#include "Node.h"
#include "Defines.h"

#include <stack>

namespace Chess {

    class BTree {
    public:
        //Initializes the root to nullptr
        BTree();

        //Deletes the BTree
        ~BTree();

        //Inserts the given ZobristHash into the BTree
        void insertHash(ZobristHash *pointer);

        //Inserts a hash into the B Tree
        void insertHash(hash Data, NextMove bestMove);

        //Finds a hash recursively using a helper function.
        ZobristHash *findHash(hash Data);

        //Recursive helper to find the hash
        ZobristHash *findHash(hash Data, Node *node);

    private:
        //Stores the root node
        Node *root;
    };

}
