#include "BTree.h"

namespace Chess {

    //Initializes the root to nullptr
    BTree::BTree() {
        root = nullptr;
    }

    //Inserts the given ZobristHash into the BTree
    void BTree::insertHash(ZobristHash *pointer) {
        //Checks if the root is non-existant
        if (root == nullptr) {
            //If so add a new one and you're done
            root = new Node();
            root->keys.push_back(pointer);
        } else {
            //Stack for recursive like tracing of the path down the tree
            std::stack<Node *> path;

            //Push the root onto the path
            path.push(root);

            Node *temp = root;

            //Traverse through the tree to the correct position to place the hash until the node is a leaf
            while (!temp->children.empty()) {
                int loc = temp->binarySearch(pointer->Data);
                temp = temp->children[loc];
                path.push(temp);
            }

            //Find the position to insert the hash
            int loc = temp->binarySearch(pointer->Data);

            //If hash exists return
            if (temp->keys.size() > loc && temp->keys[loc]->Data == pointer->Data) {
                return;
            }

            //Insert the hash into the vector of hashes
            temp->keys.insert(temp->keys.begin() + loc, pointer);

            //Traverse back up the tree and fix along the way
            while (!path.empty()) {
                //Get the node at the top of the stack and pop it off
                Node *top = path.top();
                path.pop();

                //Check if the key storage is full
                if (top->keys.size() == order) {
                    Node *t;

                    //Set if the node is a leaf or not
                    top->leaf = top->children.empty();

                    //At the top of the stack so add a new root to split into
                    if (path.empty()) {
                        t = new Node(false);
                        t->children.push_back(top);

                        root = t;
                    } else {
                        t = path.top();
                    }

                    //Get the middle point so we can split the node
                    int mid = (order % 2 == 0) ? (order / 2) - 1 : order / 2;

                    //Set the position to insert the mid point into depending on if the above node is new or not.
                    if (t->keys.empty())
                        loc = 0;
                    else
                        loc = t->binarySearch(top->keys[mid]->Data);

                    //Insert the middle value into the correct position
                    t->keys.insert(t->keys.begin() + loc, top->keys[mid]);

                    //Create a new node to store the other half of the children in after splitting
                    Node *newNode = new Node();

                    //Insert the new node into the correct child position of the above node
                    t->children.insert(t->children.begin() + loc + 1, newNode);

                    //Insert the right half of the keys into the new node
                    for (int i = mid + 1; i < top->keys.size(); i++) {
                        newNode->keys.push_back(top->keys[i]);
                    }
                    //Insert the right half of the children into the new node
                    for (int i = mid + 1; i < top->children.size(); i++) {
                        newNode->children.push_back(top->children[i]);
                    }

                    //Remove the second half of the keys from the original node since it's now split
                    top->keys.erase(top->keys.begin() + mid, top->keys.end());

                    //Reserve space again to reduce future need to do so
                    top->keys.reserve(order);

                    //Remove the second half of the children from the original node
                    if (!top->children.empty()) {
                        top->children.erase(top->children.begin() + mid + 1, top->children.end());
                    } else {
                        top->children.clear();
                    }

                    //Reserve space in the children
                    top->children.reserve(order + 1);
                }
            }
        }
    }

    //Inserts a hash into the B Tree
    void BTree::insertHash(hash Data, NextMove bestMove) {
        ZobristHash *hash = new ZobristHash(Data, bestMove);
        insertHash(hash);
    }

    //Finds a hash recursively using a helper function.
    ZobristHash *BTree::findHash(hash Data) {
        return findHash(Data, root);
    }

    //Recursive helper to find the hash
    ZobristHash *BTree::findHash(hash Data, Node *node) {
        //Hash not found
        if (node == nullptr)
            return nullptr;

        //Find correct position in keys
        int loc = node->binarySearch(Data);

        //If in that position, return the node
        if (loc < node->keys.size() && node->keys[loc]->Data == Data) {
            return node->keys[loc];
        } else {
            //Else search the children recursively
            if (node->children.size() <= loc)
                return nullptr;
            else
                return findHash(Data, node->children[loc]);
        }
    }

    //Deletes the BTree
    BTree::~BTree() {
        delete root;
    }

}