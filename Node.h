#ifndef CHESSHISTORYFINDER_NODE_H
#define CHESSHISTORYFINDER_NODE_H

#include <vector>

#include "ZobristHash.h"
#include "Defines.h"

namespace Chess {

    struct Node {
        std::vector<ZobristHash *> keys;
        std::vector<Node *> children;
        bool leaf = false;

        Node() {
            keys.reserve(m - 1);
            children.reserve(m);
        }

        //Probably going to change this logic to make it simpler
        /*bool hasCorrectChildren() {
            if ((leaf && !root) || (root && !leaf && children.size() >= 2))
                return true;
            else if (children.empty())
                return true;
            else
                return (children.size() >= ((m / 2) + 1));
        }

        bool hasCorrectKeys() {
            return keys.size() < m;
        }*/



    };

}


#endif //CHESSHISTORYFINDER_NODE_H
