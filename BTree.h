#ifndef CHESSHISTORYFINDER_BTREE_H
#define CHESSHISTORYFINDER_BTREE_H

#include "Node.h"
#include "Defines.h"

namespace Chess {

    class BTree {
    public:
        BTree();

        void insertHash(ZobristHash* pointer);

        void insertHash(hash Data, std::vector<NextMove>& bestWhite, std::vector<NextMove>& bestBlack);

        Node* findNode(hash Data);

        Node* findNode(hash Data, Node* node);

    private:
        Node* root;
    };

}


#endif //CHESSHISTORYFINDER_BTREE_H
