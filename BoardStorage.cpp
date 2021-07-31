#include "BoardStorage.h"

namespace Chess {
    BoardStorage::BoardStorage() {
//        blackToMove = false;
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
    }

    hash BoardStorage::calculateZobristHash() {
        storage = 0;

    }

    void BoardStorage::applyMove(NextMove &move) {
        bool blackToMove = !((move.move >> 14) & 0b01);
        uint8_t firstPos = (move.move >> 8) & 0b111111;
        uint8_t secondPos = (move.move >> 2) & 0b111111;
        uint8_t promotion = move.move & 0b11;

        if (!blackToMove) {
            if (board[firstPos] == Pieces::WPawn && secondPos <= 63 && secondPos >= 56) {
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
                return;
            } else if (board[firstPos] == Pieces::WPawn && board[secondPos] == Pieces::NoPiece &&
                       (secondPos == firstPos + 7 || secondPos == firstPos + 9) && secondPos >= 40 && secondPos <= 47 &&
                       board[secondPos - 8] == Pieces::BPawn) {
                board[secondPos] = Pieces::WPawn;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos - 8] = Pieces::NoPiece;
                return;
            } else if (wQueenCastle) {

            }
        } else {
            if (board[firstPos] == Pieces::BPawn && secondPos <= 7 && secondPos >= 0) {
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
                return;
            } else if (board[firstPos] == Pieces::BPawn && board[secondPos] == Pieces::NoPiece &&
                       (secondPos == firstPos - 7 || secondPos == firstPos - 9) && secondPos >= 16 && secondPos <= 23 &&
                       board[secondPos + 8] == Pieces::BPawn) {
                board[secondPos] = Pieces::BPawn;
                board[firstPos] = Pieces::NoPiece;
                board[secondPos + 8] = Pieces::NoPiece;
                return;
            }
        }

        board[secondPos] = board[firstPos];
        board[firstPos] = Pieces::NoPiece;
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
        NextMove move;
        move.move += 0x8000 * (!(blackWon() ^ blackToMove));
        move.move += 0x4000 * blackToMove;
        move.move += boardToBin(pgnMove.substr(0,2)) << 8;
        move.move += boardToBin(pgnMove.substr(2, 2)) << 2;

        return move;
    }

    uint8_t BoardStorage::boardToBin(const string &pos) {
        try {
            return (pos[0] - 'a') + (std::stoi(pos.substr(1)) - 1) * 8;
        } catch (std::exception& e) {
            std::cout << "boardToBin failed with input " << pos << std::endl;
            throw e;
        }
    }

}