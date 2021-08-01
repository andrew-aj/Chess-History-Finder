#include "ZobristHash.h"

namespace Chess {
    std::array<hash, 781> ZobristHash::randNums;

    bool ZobristHash::operator==(const ZobristHash &rhs) const {
        return Data == rhs.Data;
    }

    bool ZobristHash::operator<(const ZobristHash &rhs) const {
        return Data < rhs.Data;
    }

    bool ZobristHash::operator>(const ZobristHash &rhs) const {
        return rhs < *this;
    }

    bool ZobristHash::operator<=(const ZobristHash &rhs) const {
        return !(rhs < *this);
    }

    bool ZobristHash::operator>=(const ZobristHash &rhs) const {
        return !(*this < rhs);
    }

    ZobristHash::ZobristHash() {

    }

    void ZobristHash::generateRandomNumbers() {
        if (!initialized()){
            std::random_device rd;
            std::default_random_engine generator(rd());
            std::uniform_int_distribution<uint64_t> distribution(0, 0xFFFFFFFFFFFFFFFF);
            std::generate(randNums.begin(), randNums.end(), [&](){return distribution(generator);});
        }
    }

    ZobristHash::ZobristHash(hash data, NextMove bestMove) {
        this->Data = data;
        this->bestMove = bestMove;
    }

    bool ZobristHash::initialized() {
        return std::count(randNums.begin(), randNums.end(), 0) != 781;
    }
}