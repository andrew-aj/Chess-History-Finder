#pragma once

#include "Defines.h"
#include "ChessBinaryRep.h"
#include "ZobristHash.h"

#include <array>
#include <string>
#include <iostream>

using std::string;

namespace Chess {

    class BoardStorage {
    public:
        //Initializes the board to the default state.
        BoardStorage();

        //Calculates the Zobrist hash for the beginning of the game.
        void calculateZobristHash();

        //Calculates the zobrist hash for the current board state given castling rights and which side can move
        void calculateZobristHash(bool wKC, bool wQC, bool bKC, bool bQC, bool blackToMove);

        //Applies the move to the current board state
        void applyMove(NextMove &move);

        //Sets the board to the default position.
        void setDefault();

        //Returns the hash
        hash getHash() const;

        //Sets if white wins this game
        void setWhiteWinner();

        //Sets if black wins this game
        void setBlackWinner();

        //Sets if a draw occurs in this game
        void setDraw();

        //Returns if white wins this game
        bool whiteWon() const;

        //Returns if black wins this game
        bool blackWon() const;

        //Returns if this game results in a draw
        bool isDraw() const;

        //Generates a NextMove from a movein hyphenated notation
        NextMove generateMove(const string &pgnMove, bool blackToMove);

        //Adds a piece to the board given the piece and position
        void addPiece(Pieces pieces, int pos);

        //Resets the board state
        void clear();

        //Returns the board array
        const std::array<Pieces, 64> &getBoard() const;

    private:

        //Converts the give move in columnRow notation into a numerical position
        uint8_t boardToBin(const string &pos);

        //Calculates the index for the ZobristHash::randNums array given the location of the piece
        short calculateZobristIndex(short i);

        //Calculates the index for the ZobristHash::randNums array given the location of the piece and type
        short calculateZobristIndex(short i, Pieces piece);

        //0-7 is row 1. To move up a row, add 8 to the index.
        std::array<Pieces, 64> board;

        //Stores the hash
        hash storage;

        // 00000001 - white won
        // 00000010 - black won
        // 00000100 - draw
        uint8_t outcome;

        bool wQueenCastle;
        bool wKingCastle;

        bool bQueenCastle;
        bool bKingCastle;
    };

}
