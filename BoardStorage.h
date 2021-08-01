#pragma once

#include "Defines.h"
#include "ChessBinaryRep.h"
#include "ZobristHash.h"

#include <array>
#include <string>
#include <iostream>
using std::string;

namespace Chess {

    class BoardStorage {
    public:
        BoardStorage();

        void calculateZobristHash();

        void calculateZobristHash(bool wKC, bool wQC, bool bKC, bool bQC, bool blackToMove);

        void applyMove(NextMove& move);

        void setDefault();

//        bool isBlackMove();
//
//        void setBlackToMove(bool in);

        hash getHash() const;

        void setWhiteWinner();
        void setBlackWinner();
        void setDraw();
        bool whiteWon() const;
        bool blackWon() const;
        bool isDraw() const;

        NextMove generateMove(const string& pgnMove, bool blackToMove);

        void addPiece(Pieces pieces, int pos);

        void clear();

    private:
        uint8_t boardToBin(const string& pos);

        short calculateZobristIndex(short i);

        short calculateZobristIndex(short i, Pieces piece);

        //0-7 is row 1. To move up a row, add 8 to the index.
        std::array<Pieces, 64> board;

        hash storage;

        // 00000001 - white won
        // 00000010 - black won
        // 00000100 - draw
        uint8_t outcome;

//        bool blackToMove;

        bool wQueenCastle;
        bool wKingCastle;

        bool bQueenCastle;
        bool bKingCastle;
    };

}
