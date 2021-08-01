#pragma once

#include "ZobristHash.h"

namespace Chess {
    class Heap {
    private:
        struct HeapNode {
            ZobristHash value;
            HeapNode* parent;
            HeapNode* left;
            HeapNode* right;

            explicit HeapNode(const ZobristHash& value, HeapNode* parent = nullptr) {
                this->value = value;
                this->parent = parent;
                this->left = nullptr;
                this->right = nullptr;
            }
        };

        HeapNode* root;
        uint64_t size;
    public:
        void heapifyUp(HeapNode* node);
        void heapifyDown(HeapNode* node);
        void insert(const ZobristHash& in);
        void removeTop();
        HeapNode* nthLeaf(const uint64_t& n);
        HeapNode* getRightMostLeaf();
        HeapNode* getInsertParent();
        void swapNodeValues(HeapNode* first, HeapNode* second);

        Heap() : root(nullptr), size(0) {};
    };
}