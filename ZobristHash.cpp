#include "ZobristHash.h"

namespace Chess {

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

    }

    ZobristHash::ZobristHash(hash data, std::vector<NextMove> &bestWhite, std::vector<NextMove> &bestBlack) {
        this->Data = data;
        this->bestWhite = bestWhite;
        this->bestBlack = bestBlack;
    }
}