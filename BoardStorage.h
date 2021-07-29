#pragma once

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

        hash getHash();

    private:
        //0-7 is row 1. To move up a row, add 8 to the index.
        std::array<Pieces, 64> board;

        hash storage;
        bool whiteWon;

        bool blackToMove;

        bool wQueenCastle;
        bool wKingCastle;

        bool bQueenCastle;
        bool bKingCastle;
    };

}
