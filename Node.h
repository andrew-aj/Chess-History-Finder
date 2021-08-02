#pragma once

#include <vector>

#include "ZobristHash.h"
#include "Defines.h"

namespace Chess {

    //Stores the hashes for use in the B Tree
    struct Node {
        //Keys and Children storage
        std::vector<ZobristHash *> keys;
        std::vector<Node *> children;

        //Is this node a leaf
        bool leaf;

        //Default constructor
        Node() : Node(order, true) {

        }

        Node(bool l) : Node(order, l) {

        }

        //Sets the size of the keys and children vectors
        Node(int size, bool l) {
            keys.reserve(size);
            children.reserve(size + 1);
            leaf = l;
        }

        //Deletes the node
        ~Node() {
            for (int i = 0; i < keys.size(); i++) {
                delete keys[i];
            }

            //Recursively deletes the children
            for (int i = 0; i < children.size(); i++) {
                if (children[i]) {
                    delete children[i];
                }
            }
        }

        //Binary search to find the position that the data should be
        int binarySearch(hash Data) {
            int low = 0;
            int high = keys.size() - 1;
            int middle = 0;

            //Searches through the vector of keys
            while (low <= high) {
                middle = (low + high) / 2;
                if (Data == keys[middle]->Data)
                    return middle;
                else if (Data > keys[middle]->Data)
                    low = middle + 1;
                else
                    high = middle - 1;
            }

            //Handles the location of the child node it should be in if it does not exist in the keys vector
            if (Data > keys[middle]->Data && middle == keys.size() - 1)
                return keys.size();
            else if (Data > keys[middle]->Data && Data < keys[middle + 1]->Data)
                return middle + 1;
            return middle;
        }

    };

}
