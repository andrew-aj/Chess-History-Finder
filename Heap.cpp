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

        // Swaps node with parent if parent is greater.
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

        // As long as both children exist.
        while (node->left != nullptr && node->right != nullptr) {
            // Node is greater than both children.
            if (node->value > node->left->value && node->value > node->right->value) {
                if (node->left->value < node->right->value) {
                    swapNodeValues(node->left, node);
                    node = node->left;
                } else {
                    swapNodeValues(node->right, node);
                    node = node->right;
                }
            } else if (node->value > node->left->value) { // Node is greater than only left child.
                swapNodeValues(node->left, node);
                node = node->left;
            } else if (node->value > node->right->value) { // Node is greater than only right child.
                swapNodeValues(node->right, node);
                node = node->right;
            } else { // Node is less than both children.
                return;
            }
        }

        // Node only has one child at this point, checks if node is greater.
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

        HeapNode *insertParent = getInsertParent();
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

        cout << "something went very wrong :(" << endl;
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

        // Sets root to bottom-right most leaf, then calls heapifyDown to maintain heap properties.
        HeapNode *rml = getRightMostLeaf();
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

    Heap::HeapNode *Heap::nthLeaf(const uint64_t &n) {
        if (root == nullptr || n > size)
            return nullptr;

        // Idea to get traversal from nth node from https://stackoverflow.com/a/51509500
        uint64_t binaryTraversal = n;
        uint64_t leftMostMask = 0x8000000000000000; // Mask for leftmost bit in n.
        HeapNode *nth = root;
        unsigned int rotateCount = 0;

        // As long as there is a 0 in the leftmost bit of n.
        while (!(binaryTraversal & leftMostMask)) {
            binaryTraversal <<= 1;
            rotateCount++;
        }

        binaryTraversal <<= 1; // Get rid of redundant 1 in beginning.
        rotateCount++;

        // Goes through remaining bits in n, going left if bit = 0 and right if bit = 1.
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

    // Bottom-right most node is the size-th node of the binary heap.
    Heap::HeapNode *Heap::getRightMostLeaf() {
        return nthLeaf(size);
    }

    // Parent of any node's index is half of that node's index.
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

    void Heap::HeapSort(vector<ZobristHash> &toSort) {
        Heap heap;

        for (const auto &iter : toSort) {
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
