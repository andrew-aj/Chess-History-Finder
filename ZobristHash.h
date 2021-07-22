

#ifndef CHESSHISTORYFINDER_ZOBRISTHASH_H
#define CHESSHISTORYFINDER_ZOBRISTHASH_H

#include <array>
#include <vector>

#include "Defines.h"
#include "NextMove.h"

namespace Chess {

    class ZobristHash {
    public:
        //Generates the random numbers to create zobrist hashes
        static void generateRandomNumbers();

        ZobristHash();

        bool operator==(const ZobristHash &rhs) const;

        bool operator<(const ZobristHash &rhs) const;

        bool operator>(const ZobristHash &rhs) const;

        bool operator<=(const ZobristHash &rhs) const;

        bool operator>=(const ZobristHash &rhs) const;

    private:
        hash Data;

        //Storing the next best moves in NextMove struct.
        std::vector<NextMove> bestWhite;
        std::vector<NextMove> bestBlack;

        //Random number storage
        static std::array<hash, 781> randNums;
    };

}


#endif //CHESSHISTORYFINDER_ZOBRISTHASH_H
