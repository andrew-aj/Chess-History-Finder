#include "BTree.h"

namespace Chess{

    BTree::BTree() {
        root = nullptr;
    }

    void BTree::insertHash(ZobristHash *pointer) {

    }

    void BTree::insertHash(hash Data, std::vector<NextMove> &bestWhite, std::vector<NextMove> &bestBlack) {
        ZobristHash* hash = new ZobristHash(Data, bestWhite, bestBlack);
        insertHash(hash);
    }

    ZobristHash *BTree::findHash(hash Data) {
        return findHash(Data, root);
    }

    ZobristHash *BTree::findHash(hash Data, Node *node) {
        if (node == nullptr)
            return nullptr;

        int loc = node->binarySearch(Data);
        if (node->keys[loc]->Data != Data){
            if (node->children.size() <= loc)
                return nullptr;
            else
                return findHash(Data, node->children[loc]);
        } else{
            return node->keys[loc];
        }
    }


}