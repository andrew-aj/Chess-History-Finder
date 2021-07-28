#ifndef CHESSHISTORYFINDER_BOARDSTORAGE_H
#define CHESSHISTORYFINDER_BOARDSTORAGE_H

#include "Defines.h"
#include "ChessBinaryRep.h"
#include "ZobristHash.h"

#include <array>

namespace Chess {

    class BoardStorage {
    public:

    private:

        std::array<Pieces, 64> board;
    };

}


#endif //CHESSHISTORYFINDER_BOARDSTORAGE_H
