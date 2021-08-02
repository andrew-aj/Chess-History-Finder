#include "BoardStorage.h"

namespace Chess {
    //Initializes the board to the default state.
    BoardStorage::BoardStorage() {
        setDefault();

        calculateZobristHash();
    }

    //Calculates the Zobrist hash for the beginning of the game.
    void BoardStorage::calculateZobristHash() {
        for (int i = 0; i < board.size(); i++) {

            if (board[i] == Pieces::NoPiece)
                continue;

            storage ^= ZobristHash::randNums[calculateZobristIndex(i)];
        }

        //Side to move is white so no hashing for that.
        for (int i = 769; i < 773; i++) {
            storage ^= ZobristHash::randNums[i];
        }
    }

    //Applies the move to the current board state
    void BoardStorage::applyMove(NextMove &move) {
        //Gets the information about the next move from the binary bits.
        //Refer to NextMove.h or Defines.h for specifications.
        bool blackToMove = ((move >> 14) & 0b01);
        uint8_t firstPos = (move >> 8) & 0b111111;
        uint8_t secondPos = (move >> 2) & 0b111111;
        uint8_t promotion = move & 0b11;

        storage ^= ZobristHash::randNums[768];

        //Dealing with white pieces.
        if (!blackToMove) {
            if (board[firstPos] == Pieces::WPawn && secondPos <= 63 && secondPos >= 56) {
                //Dealing with promotions.
                //This checks if the white pawn is moving from row 7 to row 8 where promotion occurs.

                //Removing white pawn from initial position
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                //Removing piece from second position if one is there.
                if (board[secondPos] != Pieces::NoPiece) {
                    storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                }

                //Switching the board states.
                switch (promotion) {
                    case 0b00:
                        board[firstPos] = Pieces::NoPiece;
                        board[secondPos] = Pieces::WQueen;
                        break;
                    case 0b01:
                        board[firstPos] = Pieces::NoPiece;
                        board[secondPos] = Pieces::WRook;
                        break;
                    case 0b10:
                        board[firstPos] = Pieces::NoPiece;
                        board[secondPos] = Pieces::WKnight;
                        break;
                    case 0b11:
                        board[firstPos] = Pieces::NoPiece;
                        board[secondPos] = Pieces::WBishop;
                        break;
                }

                //Adding in the hash depending on which piece is promoted to.
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];

                //Stops this function since no more can be done.
                return;
            } else if (board[firstPos] == Pieces::WPawn && board[secondPos] == Pieces::NoPiece &&
                       (secondPos == firstPos + 7 || secondPos == firstPos + 9) && secondPos >= 40 && secondPos <= 47 &&
                       board[secondPos - 8] == Pieces::BPawn) {
                //Dealing with en passant. This checks if a black pawn moved 2 squares and the white pawn is right next
                //to it so it can take diagonally.

                //Removing the two pieces from the hash.
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos - 8)];

                //Removing the two pieces from the board state and setting the pawn to the correct space.
                board[secondPos] = Pieces::WPawn;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos - 8] = Pieces::NoPiece;

                //Adds the piece to the hash.
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];

                return;
            } else if (wQueenCastle && firstPos == 4 && secondPos == 2 && board[firstPos] == Pieces::WKing &&
                       board[0] == Pieces::WRook) {
                //Checking if white queen side castling is possible.

                //Sets castling rights for both sides to false.
                wQueenCastle = false;
                wKingCastle = false;

                //Removes castling rights from the hash.
                storage ^= ZobristHash::randNums[769];
                storage ^= ZobristHash::randNums[770];

                //Removes the pieces from the hash.
                storage ^= ZobristHash::randNums[calculateZobristIndex(0)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                //Sets the board state.
                board[0] = Pieces::NoPiece;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos] = Pieces::WKing;
                board[secondPos + 1] = Pieces::WRook;

                //Adds the pieces back into the hash with their new positions.
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos + 1)];

                return;
            } else if (wKingCastle && firstPos == 4 && secondPos == 6 && board[firstPos] == Pieces::WKing &&
                       board[7] == Pieces::WRook) {
                //Checks if white king side castling is possible.

                //Sets the castling rights to false.
                wQueenCastle = false;
                wKingCastle = false;

                //Removes castling rights from the hash.
                storage ^= ZobristHash::randNums[769];
                storage ^= ZobristHash::randNums[770];

                //Removes the pieces from the hash.
                storage ^= ZobristHash::randNums[calculateZobristIndex(7)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                //Sets the board state.
                board[7] = Pieces::NoPiece;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos] = Pieces::WKing;
                board[secondPos - 1] = Pieces::WRook;

                //Adds the pieces back into the hash with their new positions.
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos - 1)];

                return;
            } else if (board[firstPos] == Pieces::WKing) {
                //Removes castling rights if the king moves.
                if (wKingCastle)
                    storage ^= ZobristHash::randNums[770];

                if (wQueenCastle)
                    storage ^= ZobristHash::randNums[769];

                wKingCastle = false;
                wQueenCastle = false;
            } else if (board[firstPos] == Pieces::WRook && (wQueenCastle || wKingCastle)) {
                //Removes castling rights for a side depending on if a rook moves.
                if (firstPos == 0 && wQueenCastle) {
                    wQueenCastle = false;
                    storage ^= ZobristHash::randNums[769];
                } else if (firstPos == 7 && wKingCastle) {
                    wKingCastle = false;
                    storage ^= ZobristHash::randNums[770];
                }
            } else if (board[secondPos] == Pieces::BRook && (bQueenCastle || bKingCastle)) {
                //Removes castling rights for one of the black sides if a white piece captures that
                //black rook.
                if (firstPos == 56 && bQueenCastle) {
                    bQueenCastle = false;
                    storage ^= ZobristHash::randNums[771];
                } else if (firstPos == 63 && bKingCastle) {
                    bKingCastle = false;
                    storage ^= ZobristHash::randNums[772];
                }
            }
        } else {
            if (board[firstPos] == Pieces::BPawn && secondPos <= 7 && secondPos >= 0) {
                //Dealing with promotions.
                //This checks if the white pawn is moving from row 7 to row 8 where promotion occurs.

                //Removing white pawn from initial position
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                //Removing piece from second position if one is there.
                if (board[secondPos] != Pieces::NoPiece) {
                    storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                }

                //Switching the board states.
                switch (promotion) {
                    case 0b00:
                        board[firstPos] = Pieces::NoPiece;
                        board[secondPos] = Pieces::BQueen;
                        break;
                    case 0b01:
                        board[firstPos] = Pieces::NoPiece;
                        board[secondPos] = Pieces::BRook;
                        break;
                    case 0b10:
                        board[firstPos] = Pieces::NoPiece;
                        board[secondPos] = Pieces::BKnight;
                        break;
                    case 0b11:
                        board[firstPos] = Pieces::NoPiece;
                        board[secondPos] = Pieces::BBishop;
                        break;
                }

                //Adding in the hash depending on which piece is promoted to.
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];

                //Stops the function since no more can be done.
                return;
            } else if (board[firstPos] == Pieces::BPawn && board[secondPos] == Pieces::NoPiece &&
                       (secondPos == firstPos - 7 || secondPos == firstPos - 9) && secondPos >= 16 && secondPos <= 23 &&
                       board[secondPos + 8] == Pieces::BPawn) {
                //Dealing with en passant. This checks if a white pawn moved 2 squares and the black pawn is right next
                //to it so it can take diagonally.

                //Removing the two pieces from the hash.
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos + 8)];

                //Removing the two pieces from the board state and setting the pawn to the correct space.
                board[secondPos] = Pieces::BPawn;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos + 8] = Pieces::NoPiece;

                //Adds the piece to the hash.
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];

                return;
            } else if (bQueenCastle && firstPos == 60 && secondPos == 58 && board[firstPos] == Pieces::BKing &&
                       board[56] == Pieces::BRook) {
                //Checking if black queen side castling is possible.

                //Sets castling rights for both sides to false.
                bQueenCastle = false;
                bKingCastle = false;

                //Removes castling rights from the hash.
                storage ^= ZobristHash::randNums[771];
                storage ^= ZobristHash::randNums[772];

                //Removes the pieces from the hash.
                storage ^= ZobristHash::randNums[calculateZobristIndex(56)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                //Sets the board state.
                board[56] = Pieces::NoPiece;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos] = Pieces::BKing;
                board[secondPos + 1] = Pieces::BRook;

                //Adds the pieces back into the hash with their new positions.
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos + 1)];

                return;
            } else if (bKingCastle && firstPos == 60 && secondPos == 62 && board[firstPos] == Pieces::BKing &&
                       board[63] == Pieces::BRook) {
                //Checks if black king side castling is possible.

                //Sets the castling rights to false.
                bQueenCastle = false;
                bKingCastle = false;

                //Removes castling rights from the hash.
                storage ^= ZobristHash::randNums[771];
                storage ^= ZobristHash::randNums[772];

                //Removes the pieces from the hash.
                storage ^= ZobristHash::randNums[calculateZobristIndex(63)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                //Sets the board state.
                board[63] = Pieces::NoPiece;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos] = Pieces::BKing;
                board[secondPos - 1] = Pieces::BRook;

                //Adds the pieces back into the hash with their new positions.
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos - 1)];

                return;
            } else if (board[firstPos] == Pieces::BKing) {
                //Removes castling rights if the king moves.
                if (bKingCastle)
                    storage ^= ZobristHash::randNums[772];

                if (bQueenCastle)
                    storage ^= ZobristHash::randNums[771];

                bKingCastle = false;
                bQueenCastle = false;
            } else if (board[firstPos] == Pieces::BRook && (bQueenCastle || bKingCastle)) {
                //Removes castling rights for a side depending on if a rook moves.
                if (firstPos == 56 && bQueenCastle) {
                    bQueenCastle = false;
                    storage ^= ZobristHash::randNums[771];
                } else if (firstPos == 63 && bKingCastle) {
                    bKingCastle = false;
                    storage ^= ZobristHash::randNums[772];
                }
            } else if (board[secondPos] == Pieces::WRook && (wQueenCastle || wKingCastle)) {
                //Removes castling rights for one of the white sides if a black piece captures that
                //white rook.
                if (firstPos == 0 && wQueenCastle) {
                    wQueenCastle = false;
                    storage ^= ZobristHash::randNums[769];
                } else if (firstPos == 7 && wKingCastle) {
                    wKingCastle = false;
                    storage ^= ZobristHash::randNums[770];
                }
            }
        }

        //This is for normal moves.

        //Removes the piece from the hash
        storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

        //Removes the second piece from the hash if one exists.
        if (board[secondPos] != Pieces::NoPiece) {
            storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
        }

        //Changes the board state.
        board[secondPos] = board[firstPos];
        board[firstPos] = Pieces::NoPiece;

        //Updates the hash
        storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
    }

    //Returns the hash
    hash BoardStorage::getHash() const {
        return storage;
    }

    //Sets if white wins this game
    void BoardStorage::setWhiteWinner() {
        outcome = 0b001;
    }

    //Sets if black wins this game
    void BoardStorage::setBlackWinner() {
        outcome = 0b010;
    }

    //Sets if a draw occurs in this game
    void BoardStorage::setDraw() {
        outcome = 0b100;
    }

    //Returns if white wins this game
    bool BoardStorage::whiteWon() const {
        if (outcome == 0b001)
            return true;

        return false;
    }

    //Returns if black wins this game
    bool BoardStorage::blackWon() const {
        if (outcome == 0b010)
            return true;

        return false;
    }

    //Returns if this game results in a draw
    bool BoardStorage::isDraw() const {
        if (outcome == 0b100)
            return true;

        return false;
    }

    //Generates a NextMove from a movein hyphenated notation
    NextMove BoardStorage::generateMove(const string &pgnMove, bool blackToMove) {
        NextMove move = 0;

        //Initializes first bit
        if (isDraw()) {
            move += 0x8000;
        } else {
            move += 0x8000 * (!(blackWon() ^ blackToMove));
        }

        //Initializes second bit
        move += 0x4000 * blackToMove;

        //For a normal move
        if (pgnMove.find('-') != string::npos) {
            //Sets the next 12 bits depending on the move
            move += boardToBin(pgnMove.substr(0, 2)) << 8;
            move += boardToBin(pgnMove.substr(3, 2)) << 2;
            //Last 2 bits are empty because no promotion
        } else {
            //For if a promotion occurs

            //Sets the next 12 bits depending on the move
            move += boardToBin(pgnMove.substr(0, 2)) << 8;
            move += boardToBin(pgnMove.substr(2, 2)) << 2;

            //Sets the last 2 bits to the correct promotion type.
            char promotion = pgnMove[4];
            switch (promotion) {
                case 'Q':
                    move += 0;
                    break;
                case 'R':
                    move += 1;
                    break;
                case 'N':
                    move += 2;
                    break;
                case 'B':
                    move += 3;
                    break;
                default:
                    move += 0;
                    break;
            }
        }

        return move;
    }

    //Converts the give move in columnRow notation into a numerical position
    uint8_t BoardStorage::boardToBin(const string &pos) {
        try {
            return (pos[0] - 'a') + (std::stoi(pos.substr(1)) - 1) * 8;
        } catch (std::exception &e) {
            std::cout << "boardToBin failed with input " << pos << std::endl;
            throw e;
        }
    }

    //Calculates the index for the ZobristHash::randNums array given the location of the piece
    short BoardStorage::calculateZobristIndex(short i) {
        short square = i * 12;
        short index = (board[i] >= Pieces::BPawn) ? board[i] - 2 : board[i];
        return square + index;
    }

    //Calculates the index for the ZobristHash::randNums array given the location of the piece and type
    short BoardStorage::calculateZobristIndex(short i, Pieces piece) {
        short square = i * 12;
        short index = (piece >= Pieces::BPawn) ? piece - 2 : piece;
        return square + index;
    }

    //Adds a piece to the board given the piece and position
    void BoardStorage::addPiece(Pieces pieces, int pos) {
        board[pos] = pieces;
    }

    //Resets the board state
    void BoardStorage::clear() {
        storage = 0;
        wQueenCastle = true;
        wKingCastle = true;
        bQueenCastle = true;
        bKingCastle = true;
        board.fill(Pieces::NoPiece);
    }

    //Calculates the zobrist hash for the current board state given castling rights and which side can move
    void BoardStorage::calculateZobristHash(bool wKC, bool wQC, bool bKC, bool bQC, bool blackToMove) {
        //Resets hash
        storage = 0;

        //Sets the hash for each piece in each location
        for (int i = 0; i < board.size(); i++) {
            if (board[i] == Pieces::NoPiece)
                continue;
            storage ^= ZobristHash::randNums[calculateZobristIndex(i)];
        }

        //Adds the hash if it's black's turn to move
        if (blackToMove) {
            storage ^= ZobristHash::randNums[768];
        }

        //Adds the castling rights to the hash
        if (wQC)
            storage ^= ZobristHash::randNums[769];
        if (wKC)
            storage ^= ZobristHash::randNums[770];
        if (bQC)
            storage ^= ZobristHash::randNums[771];
        if (bKC)
            storage ^= ZobristHash::randNums[772];

    }

    //Sets the board to the default position.
    void BoardStorage::setDefault() {
        storage = 0;

        board.fill(Pieces::NoPiece);
        board[0] = Pieces::WRook;
        board[1] = Pieces::WKnight;
        board[2] = Pieces::WBishop;
        board[3] = Pieces::WQueen;
        board[4] = Pieces::WKing;
        board[5] = Pieces::WBishop;
        board[6] = Pieces::WKnight;
        board[7] = Pieces::WRook;

        for (int i = 56; i < 64; i++) {
            board[i] = static_cast<Pieces>(board[i - 56] + 0b1000);
        }

        for (int i = 8; i < 16; i++) {
            board[i] = Pieces::WPawn;
        }

        for (int i = 48; i < 56; i++) {
            board[i] = Pieces::BPawn;
        }

        wQueenCastle = true;
        wKingCastle = true;

        bQueenCastle = true;
        bKingCastle = true;
    }

    //Returns the board array
    const std::array<Pieces, 64> &BoardStorage::getBoard() const {
        return board;
    }

}