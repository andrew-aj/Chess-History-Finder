

#ifndef CHESSHISTORYFINDER_ZOBRISTHASH_H
#define CHESSHISTORYFINDER_ZOBRISTHASH_H

#include <array>

#include "Defines.h"

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

        //Random number storage
        static std::array<hash, 781> randNums;
    };

}


#endif //CHESSHISTORYFINDER_ZOBRISTHASH_H
