#include "BoardStorage.h"

namespace Chess {


    BoardStorage::BoardStorage() {
        blackToMove = false;
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
    }

    hash BoardStorage::calculateZobristHash() {
        return 0;
    }

    void BoardStorage::applyMove(NextMove &move) {
        blackToMove = !(move.move >> 14);
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
            }
        }

        board[secondPos] = board[firstPos];
        board[firstPos] = Pieces::NoPiece;
    }

    bool BoardStorage::isBlackMove() {
        return blackToMove;
    }

    void BoardStorage::setBlackToMove(bool in) {
        blackToMove = in;
    }
}