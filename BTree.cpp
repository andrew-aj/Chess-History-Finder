#include "BTree.h"

namespace Chess {

    BTree::BTree() {
        root = nullptr;
    }

    void BTree::insertHash(ZobristHash *pointer) {
        if (root == nullptr) {
            root = new Node();
            root->keys.push_back(pointer);
        } else {
            std::stack<Node *> path;
            path.push(root);

            Node *temp = root;

            while (!temp->leaf) {
                int loc = temp->binarySearch(pointer->Data);
                temp = temp->children[loc];
                path.push(temp);
            }

            int loc = temp->binarySearch(pointer->Data);
            temp->keys.insert(temp->keys.begin() + loc, pointer);

            while (!path.empty()) {
                Node *top = path.top();
                path.pop();

                if (top->keys.size() == order) {
                    Node *t;
                    top->leaf = top->children.empty();
                    if (path.empty()) {
                        t = new Node(false);
                        t->children.push_back(top);
                        root = t;
                    } else {
                        t = path.top();
                    }
                    int mid = (order % 2 == 0) ? (order / 2) - 1 : order / 2;
                    if (t->keys.empty())
                        loc = 0;
                    else
                        loc = t->binarySearch(temp->keys[mid]->Data);
                    t->keys.insert(t->keys.begin() + loc, temp->keys[mid]);
                    Node *newNode = new Node();
                    t->children.insert(t->children.begin() + loc + 1, newNode);
                    //std::copy(temp->keys.begin() + mid + 1, temp->keys.end(), newNode->keys.begin()); why no work
                    for(int i = mid + 1; i < temp->keys.size(); i++){
                        newNode->keys.push_back(temp->keys[i]);
                    }
                    temp->keys.erase(temp->keys.begin() + mid, temp->keys.end());
                    temp->keys.reserve(order);
                }
            }
        }
    }

    void BTree::insertHash(hash Data, std::vector<NextMove> &bestWhite, std::vector<NextMove> &bestBlack) {
        ZobristHash *hash = new ZobristHash(Data, bestWhite, bestBlack);
        insertHash(hash);
    }

    ZobristHash *BTree::findHash(hash Data) {
        return findHash(Data, root);
    }

    ZobristHash *BTree::findHash(hash Data, Node *node) {
        if (node == nullptr)
            return nullptr;

        int loc = node->binarySearch(Data);
        if (node->keys[loc]->Data != Data) {
            if (node->children.size() <= loc)
                return nullptr;
            else
                return findHash(Data, node->children[loc]);
        } else {
            return node->keys[loc];
        }
    }


}