#pragma once

#include <cstdint>

namespace Chess {

    // An int with a fixed width of 64 bits.
    using hash = uint64_t;

    //Stored in fixed width int using 16 bits.
    //Format: 0000000000000000
    //First bit stores who won depending on the next bit. (a win could also indicate a draw)
    //Next bit represents whether move is white (0) or black (1)
    //Next 6 bits represent from tile
    //Next 6 bits represent to tile.
    //Last 2 bits represent promotion if a pawn is moving to the opposite end of the board.
    //For promotion, 00 = Queen, 01 = Rook, 10 = Knight, 11 = Bishop
    using NextMove = uint16_t;

    // The order of the B-tree.
    #define order 5

}
