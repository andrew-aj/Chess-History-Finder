#ifndef CHESSHISTORYFINDER_BOARDSTORAGE_H
#define CHESSHISTORYFINDER_BOARDSTORAGE_H

#include "Defines.h"
#include "ChessBinaryRep.h"
#include "ZobristHash.h"

#include <array>

namespace Chess {

    class BoardStorage {
    public:
        BoardStorage();

        hash calculateZobristHash();

        void applyMove(NextMove& move);

        bool isBlackMove();

        void setBlackToMove(bool in);

    private:
        //0-7 is row 1. To move up a row, add 8 to the index.
        std::array<Pieces, 64> board;

        hash storage;

        bool blackToMove;

        bool wQueenCastle;
        bool wKingCastle;

        bool bQueenCastle;
        bool bKingCastle;
    };

}


#endif //CHESSHISTORYFINDER_BOARDSTORAGE_H
