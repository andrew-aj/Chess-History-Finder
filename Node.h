#pragma once

#include <vector>

#include "ZobristHash.h"
#include "Defines.h"

namespace Chess {

    struct Node {
        std::vector<ZobristHash *> keys;
        std::vector<Node *> children;
        bool leaf;

        Node() : Node(order, true) {

        }

        Node(bool l) : Node(order, l) {

        }

        Node(int size, bool l) {
            keys.reserve(size);
            children.reserve(size+1);
            leaf = l;
        }

        int binarySearch(hash Data) {
            int low = 0;
            int high = keys.size() - 1;
            int middle = 0;
            while (low <= high) {
                middle = (low + high) / 2;
                if (Data == keys[middle]->Data)
                    return middle;
                else if (Data > keys[middle]->Data)
                    low = middle + 1;
                else
                    high = middle - 1;
            }
            if (Data > keys[middle]->Data && middle == keys.size() - 1)
                return keys.size();
            else if (Data > keys[middle]->Data && Data < keys[middle + 1]->Data)
                return middle + 1;
            return middle;
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
