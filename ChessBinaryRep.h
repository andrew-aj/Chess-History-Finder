#ifndef CHESSHISTORYFINDER_CHESSBINARYREP_H
#define CHESSHISTORYFINDER_CHESSBINARYREP_H

#include <cstdint>

namespace Chess {

    enum Pieces: uint8_t {
        WPawn = 0b0000,
        WRook = 0b0001,
        WKnight = 0b0010,
        WBishop = 0b0011,
        WQueen = 0b0100,
        WKing = 0b0101,
        BPawn = 0b1000,
        BRook = 0b1001,
        BKnight = 0b1010,
        BBishop = 0b1011,
        BQueen = 0b1100,
        BKing = 0b1101,
        NoPiece = 0b1111
    };

}

#endif //CHESSHISTORYFINDER_CHESSBINARYREP_H
