

#ifndef CHESSHISTORYFINDER_NEXTMOVE_H
#define CHESSHISTORYFINDER_NEXTMOVE_H

namespace Chess {

    struct NextMove {
        //Store in short using 14 bits.
        //Format: 0011111111111111
        //First 2 bits are empty.
        //Next 6 bits represent from tile
        //Next 6 bits represent to tile.
        //Last 2 bits represent promotion if a pawn is moving to the opposite end of the board.
        short move;
        float whitePercent;
        float blackPercent;
    };

}


#endif //CHESSHISTORYFINDER_NEXTMOVE_H
