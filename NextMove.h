#pragma once

namespace Chess {

    struct NextMove {
        //Store in short using 14 bits.
        //Format: 0111111111111111
        //First bit is empty.
        //Next bit represents whether move is white (0) or black (1)
        //Next 6 bits represent from tile
        //Next 6 bits represent to tile.
        //Last 2 bits represent promotion if a pawn is moving to the opposite end of the board.
        short move;
        float whitePercent;
        float blackPercent;
    };

}
