#pragma once

#include "ZobristHash.h"
#include <vector>

using std::vector;

namespace Chess {
    class Heap {
    private:
        struct HeapNode {
            ZobristHash value;
            HeapNode *parent;
            HeapNode *left;
            HeapNode *right;

            explicit HeapNode(const ZobristHash &value, HeapNode *parent = nullptr) {
                this->value = value;
                this->parent = parent;
                this->left = nullptr;
                this->right = nullptr;
            }
        };

        HeapNode *root;
        uint64_t size;
    public:
        void heapifyUp(HeapNode *node);

        void heapifyDown(HeapNode *node);

        void insert(const ZobristHash &in);

        void removeTop();

        // Returns the nth node in the level traversal of the binary heap.
        HeapNode *nthLeaf(const uint64_t &n);

        // Returns the bottom-right most leaf in the tree.
        HeapNode *getRightMostLeaf();

        // Returns the parent of the node that would be inserted next.
        HeapNode *getInsertParent();

        // Swaps only the values of two nodes.
        void swapNodeValues(HeapNode *first, HeapNode *second);

        bool isEmpty();

        // Returns the node at the top of the binary heap.
        ZobristHash top();

        // Constructs a heap from passed in vector, clears vector, and then pushes sorted values back into vector.
        static void HeapSort(vector<ZobristHash> &toSort);

        Heap() : root(nullptr), size(0) {};

        ~Heap();
    };
}