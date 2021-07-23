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

    Node *BTree::findNode(hash Data) {
        return findNode(Data, root);
    }

    Node *BTree::findNode(hash Data, Node *node) {
        if (node == nullptr)
            return nullptr;

        int loc = 0;
        for (int i = 0; i < node->keys.size(); i++) {
            if (node->keys[i]->Data >= Data){
                
            }
        }
    }


}