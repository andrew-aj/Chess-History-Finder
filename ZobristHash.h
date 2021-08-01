#pragma once

#include <array>
#include <vector>
#include <algorithm>
#include <random>

#include "Defines.h"
#include "NextMove.h"

namespace Chess {

    struct ZobristHash {
        hash Data;

        //Storing the next best moves in NextMove struct.
        /*std::vector<NextMove> bestWhite;
        std::vector<NextMove> bestBlack;*/
        NextMove bestMove;

        //Random number storage
        //0-11 are the pieces on a1 and so on in the same order as the Pieces enum.
        //index 768 is move hash.
        //index 769-772 is castling rights
        //For castling, 769 is WQ castle, 770 is WK castle, 771 is BQ castle, 772 is BK castle
        //773-780 are the en passant files.
        static std::array<hash, 781> randNums;

        //Generates the random numbers to create zobrist hashes
        static void generateRandomNumbers();

        static bool initialized();

        ZobristHash();

        ZobristHash(hash data, NextMove bestMove);

        bool operator==(const ZobristHash &rhs) const;

        bool operator<(const ZobristHash &rhs) const;

        bool operator>(const ZobristHash &rhs) const;

        bool operator<=(const ZobristHash &rhs) const;

        bool operator>=(const ZobristHash &rhs) const;
    };

}
