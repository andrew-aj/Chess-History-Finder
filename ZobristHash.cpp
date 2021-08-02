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

    std::string ZobristHash::hashToMove() {
        std::string hyphenatedMove;
        uint8_t from = (bestMove >> 8) & 0b111111;
        hyphenatedMove += ((from + 1) % 8) + 'a';
        hyphenatedMove += ((from + 1) / 8) + '1';
        uint8_t to = (bestMove >> 2) & 0b111111;
        hyphenatedMove += '-';
        hyphenatedMove += ((to + 1) % 8) + 'a';
        hyphenatedMove += ((to + 1) / 8) + '1';
        return hyphenatedMove;
    }
}