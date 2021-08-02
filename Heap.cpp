#include "Heap.h"
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

namespace Chess {
    void Heap::heapifyUp(HeapNode *node) {
        if (node == nullptr) {
            cout << "nullptr passed into heapifyUp" << endl;
            return;
        }

        while (node->parent != nullptr && node->parent->value > node->value) {
            swapNodeValues(node->parent, node);
            node = node->parent;
        }
    }

    void Heap::heapifyDown(HeapNode *node) {
        if (node == nullptr) {
            cout << "nullptr passed into heapifyDown" << endl;
            return;
        }

        while (node->left != nullptr && node->right != nullptr) {
            if (node->value > node->left->value && node->value > node->right->value) { // greater than both children
                if (node->left->value < node->right->value) {
                    swapNodeValues(node->left, node);
                    node = node->left;
                } else {
                    swapNodeValues(node->right, node);
                    node = node->right;
                }
            } else if (node->value > node->left->value) {
                swapNodeValues(node->left, node);
                node = node->left;
            } else if (node->value > node->right->value) {
                swapNodeValues(node->right, node);
                node = node->right;
            } else { // less than both children
                return;
            }
        }

        if (node->left != nullptr && node->value > node->left->value) {
            swapNodeValues(node->left, node);
            node = node->left;
        }
    }



    void Heap::insert(const Chess::ZobristHash &value) {
        if (root == nullptr) {
            root = new HeapNode(value);
            size++;
            return;
        }

        HeapNode* insertParent = getInsertParent();
        if (insertParent->left == nullptr) {
            insertParent->left = new HeapNode(value, insertParent);
            heapifyUp(insertParent->left);
            size++;
            return;
        }

        if (insertParent->right == nullptr) {
            insertParent->right = new HeapNode(value, insertParent);
            heapifyUp(insertParent->right);
            size++;
            return;
        }

        cout << "something went very wrong :("  << endl;
    }

    void Heap::removeTop() {
        if (root == nullptr) {
            return;
        }

        if (size == 1) {
            delete root;
            root = nullptr;
            size--;
            return;
        }

        HeapNode* rml = getRightMostLeaf();
        root->value = rml->value;
        if (rml == rml->parent->left) {
            rml->parent->left = nullptr;
        } else if (rml == rml->parent->right) {
            rml->parent->right = nullptr;
        } else {
            cout << "something went wrong with removeTop()" << endl;
        }

        delete rml;
        size--;
        heapifyDown(root);
    }

    Heap::HeapNode *Heap::nthLeaf(const uint64_t& n) {
        if (root == nullptr || n > size)
            return nullptr;

        uint64_t binaryTraversal = n; // Traversal method found at https://stackoverflow.com/questions/51506395/how-can-one-find-the-last-right-most-node-on-the-last-level-of-tree-which-is-a
        uint64_t leftMostMask = 0x8000000000000000;
        HeapNode* nth = root;
        unsigned int rotateCount = 0;

        while (!(binaryTraversal & leftMostMask)) {
            binaryTraversal <<= 1;
            rotateCount++;
        }

        binaryTraversal <<= 1; // Get rid of redundant 1 in beginning.
        rotateCount++;

        while (64 - rotateCount != 0) {
            if (binaryTraversal & leftMostMask)
                nth = nth->right;
            else
                nth = nth->left;

            binaryTraversal <<= 1;
            rotateCount++;
        }

        return nth;
    }

    Heap::HeapNode *Heap::getRightMostLeaf() {
        return nthLeaf(size);
    }

    Heap::HeapNode *Heap::getInsertParent() {
        return nthLeaf((size + 1) / 2);
    }

    void Heap::swapNodeValues(HeapNode *first, HeapNode *second) {
        ZobristHash temp = first->value;
        first->value = second->value;
        second->value = temp;
    }

    ZobristHash Heap::top() {
        return root->value;
    }

    bool Heap::isEmpty() {
        if (size == 0)
            return true;

        return false;
    }

    void Heap::HeapSort(vector<ZobristHash>& toSort) {
        Heap heap;

        for (const auto& iter : toSort) {
            heap.insert(iter);
        }

        toSort.clear();

        while (!heap.isEmpty()) {
            toSort.push_back(heap.top());
            heap.removeTop();
        }
    }

    Heap::~Heap() {
        while (!this->isEmpty())
            this->removeTop();
    }

}
