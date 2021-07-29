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
        std::vector<NextMove> bestWhite;
        std::vector<NextMove> bestBlack;

        //Random number storage
        //
        static std::array<hash, 781> randNums;

        //Generates the random numbers to create zobrist hashes
        static void generateRandomNumbers();

        static bool initialized();

        ZobristHash();

        ZobristHash(hash data, std::vector<NextMove>& bestWhite, std::vector<NextMove>& bestBlack);

        bool operator==(const ZobristHash &rhs) const;

        bool operator<(const ZobristHash &rhs) const;

        bool operator>(const ZobristHash &rhs) const;

        bool operator<=(const ZobristHash &rhs) const;

        bool operator>=(const ZobristHash &rhs) const;
    };

}
