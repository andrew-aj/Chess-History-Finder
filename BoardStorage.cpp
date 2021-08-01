#include "BoardStorage.h"

namespace Chess {
    BoardStorage::BoardStorage() {
//        blackToMove = false;
        storage = 0;
        board.fill(Pieces::NoPiece);
        board[0] = Pieces::WRook;
        board[1] = Pieces::WKnight;
        board[2] = Pieces::WBishop;
        board[3] = Pieces::WQueen;
        board[4] = Pieces::WKing;
        board[5] = Pieces::WBishop;
        board[6] = Pieces::WKing;
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

        calculateZobristHash();
    }

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

    void BoardStorage::applyMove(NextMove &move) {
        bool blackToMove = ((move >> 14) & 0b01);
        uint8_t firstPos = (move >> 8) & 0b111111;
        uint8_t secondPos = (move >> 2) & 0b111111;
        uint8_t promotion = move & 0b11;

        storage ^= ZobristHash::randNums[768];

        if (!blackToMove) {
            if (board[firstPos] == Pieces::WPawn && secondPos <= 63 && secondPos >= 56) {
                //Dealing with promotions.
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];
                if (board[secondPos] != Pieces::NoPiece) {
                    storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                }

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

                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];

                return;
            } else if (board[firstPos] == Pieces::WPawn && board[secondPos] == Pieces::NoPiece &&
                       (secondPos == firstPos + 7 || secondPos == firstPos + 9) && secondPos >= 40 && secondPos <= 47 &&
                       board[secondPos - 8] == Pieces::BPawn) {
                //Dealing with en passant
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos - 8)];

                board[secondPos] = Pieces::WPawn;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos - 8] = Pieces::NoPiece;

                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];

                return;
            } else if (wQueenCastle && firstPos == 4 && secondPos == 2 && board[firstPos] == Pieces::WKing &&
                       board[0] == Pieces::WRook) {
                //Checking if white queen side castling is possible.

                wQueenCastle = false;
                wKingCastle = false;

                storage ^= ZobristHash::randNums[769];
                storage ^= ZobristHash::randNums[770];

                storage ^= ZobristHash::randNums[calculateZobristIndex(0)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                board[0] = Pieces::NoPiece;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos] = Pieces::WKing;
                board[secondPos + 1] = Pieces::WRook;

                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos + 1)];

                return;
            } else if (wKingCastle && firstPos == 4 && secondPos == 6 && board[firstPos] == Pieces::WKing &&
                       board[7] == Pieces::WRook) {
                wQueenCastle = false;
                wKingCastle = false;

                storage ^= ZobristHash::randNums[769];
                storage ^= ZobristHash::randNums[770];

                storage ^= ZobristHash::randNums[calculateZobristIndex(7)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                board[7] = Pieces::NoPiece;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos] = Pieces::WKing;
                board[secondPos - 1] = Pieces::WRook;

                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos - 1)];

                return;
            } else if (board[firstPos] == Pieces::WKing) {
                if (wKingCastle)
                    storage ^= ZobristHash::randNums[770];
                if (wQueenCastle)
                    storage ^= ZobristHash::randNums[769];
                wKingCastle = false;
                wQueenCastle = false;
            } else if (board[firstPos] == Pieces::WRook && (wQueenCastle || wKingCastle)) {
                if (firstPos == 0 && wQueenCastle) {
                    wQueenCastle = false;
                    storage ^= ZobristHash::randNums[769];
                } else if (firstPos == 7 && wKingCastle) {
                    wKingCastle = false;
                    storage ^= ZobristHash::randNums[770];
                }
            } else if (board[secondPos] == Pieces::BRook && (bQueenCastle || bKingCastle)) {
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
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];
                if (board[secondPos] != Pieces::NoPiece) {
                    storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                }

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
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];

                return;
            } else if (board[firstPos] == Pieces::BPawn && board[secondPos] == Pieces::NoPiece &&
                       (secondPos == firstPos - 7 || secondPos == firstPos - 9) && secondPos >= 16 && secondPos <= 23 &&
                       board[secondPos + 8] == Pieces::BPawn) {
                //Dealing with en passant
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos + 8)];

                board[secondPos] = Pieces::BPawn;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos + 8] = Pieces::NoPiece;

                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];

                return;
            } else if (bQueenCastle && firstPos == 60 && secondPos == 58 && board[firstPos] == Pieces::BKing &&
                       board[56] == Pieces::BRook) {
                //Checking if black queen side castling is possible.

                bQueenCastle = false;
                bKingCastle = false;

                storage ^= ZobristHash::randNums[771];
                storage ^= ZobristHash::randNums[772];

                storage ^= ZobristHash::randNums[calculateZobristIndex(56)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                board[56] = Pieces::NoPiece;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos] = Pieces::BKing;
                board[secondPos + 1] = Pieces::BRook;

                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos + 1)];

                return;
            } else if (wKingCastle && firstPos == 60 && secondPos == 62 && board[firstPos] == Pieces::BKing &&
                       board[63] == Pieces::BRook) {
                bQueenCastle = false;
                bKingCastle = false;

                storage ^= ZobristHash::randNums[771];
                storage ^= ZobristHash::randNums[772];

                storage ^= ZobristHash::randNums[calculateZobristIndex(63)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];

                board[63] = Pieces::NoPiece;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos] = Pieces::BKing;
                board[secondPos - 1] = Pieces::BRook;

                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
                storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos - 1)];

                return;
            } else if (board[firstPos] == Pieces::BKing) {
                if (bKingCastle)
                    storage ^= ZobristHash::randNums[772];
                if (bQueenCastle)
                    storage ^= ZobristHash::randNums[771];
                bKingCastle = false;
                bQueenCastle = false;
            } else if (board[firstPos] == Pieces::BRook && (bQueenCastle || bKingCastle)) {
                if (firstPos == 56 && bQueenCastle) {
                    bQueenCastle = false;
                    storage ^= ZobristHash::randNums[771];
                } else if (firstPos == 63 && bKingCastle) {
                    bKingCastle = false;
                    storage ^= ZobristHash::randNums[772];
                }
            } else if (board[secondPos] == Pieces::WRook && (wQueenCastle || wKingCastle)) {
                if (firstPos == 0 && wQueenCastle) {
                    wQueenCastle = false;
                    storage ^= ZobristHash::randNums[769];
                } else if (firstPos == 7 && wKingCastle) {
                    wKingCastle = false;
                    storage ^= ZobristHash::randNums[770];
                }
            }
        }

        storage ^= ZobristHash::randNums[calculateZobristIndex(firstPos)];
        if (board[secondPos] != Pieces::NoPiece) {
            storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
        }

        board[secondPos] = board[firstPos];
        board[firstPos] = Pieces::NoPiece;

        storage ^= ZobristHash::randNums[calculateZobristIndex(secondPos)];
    }

//    bool BoardStorage::isBlackMove() {
//        return blackToMove;
//    }

//    void BoardStorage::setBlackToMove(bool in) {
//        blackToMove = in;
//    }

    hash BoardStorage::getHash() const {
        return storage;
    }

    void BoardStorage::setWhiteWinner() {
        outcome = 0b001;
    }

    void BoardStorage::setBlackWinner() {
        outcome = 0b010;
    }

    void BoardStorage::setDraw() {
        outcome = 0b100;
    }

    bool BoardStorage::whiteWon() const {
        if (outcome == 0b001)
            return true;

        return false;
    }

    bool BoardStorage::blackWon() const {
        if (outcome == 0b010)
            return true;

        return false;
    }

    bool BoardStorage::isDraw() const {
        if (outcome == 0b100)
            return true;

        return false;
    }

    NextMove BoardStorage::generateMove(const string &pgnMove, bool blackToMove) {
        NextMove move = 0;
        if (isDraw()) {
            move += 0x8000;
        } else {
            move += 0x8000 * (!(blackWon() ^ blackToMove));
        }
        move += 0x4000 * blackToMove;
        if (pgnMove.find('-') != string::npos) {
            move += boardToBin(pgnMove.substr(0, 2)) << 8;
            move += boardToBin(pgnMove.substr(3, 2)) << 2;
        } else {
            move += boardToBin(pgnMove.substr(0, 2)) << 8;
            move += boardToBin(pgnMove.substr(2, 2)) << 2;
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

    uint8_t BoardStorage::boardToBin(const string &pos) {
        if (!pos.empty()) {
            try {
                return (pos[0] - 'a') + (std::stoi(pos.substr(1)) - 1) * 8;
            } catch (std::exception &e) {
                std::cout << "boardToBin failed with input " << pos << std::endl;
                throw e;
            }
        } else {


        }
    }

    short BoardStorage::calculateZobristIndex(short i) {
        short square = i * 12;
        short index = (board[i] >= Pieces::BPawn) ? board[i] - 2 : board[i];
        return square + index;
    }

    short BoardStorage::calculateZobristIndex(short i, Pieces piece) {
        short square = i * 12;
        short index = (piece >= Pieces::BPawn) ? piece - 2 : piece;
        return square + index;
    }

}