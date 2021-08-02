#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

namespace Chess {

    //Stores the pieces in binary using an enum that is 1 byte in size for each
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

    //Converts text to the correct piece
    static std::unordered_map<std::string, Pieces> stringToPieces {
            {"WPawn", WPawn},
            {"WRook", WRook},
            {"WKnight", WKnight},
            {"WBishop", WBishop},
            {"WQueen", WQueen},
            {"WKing", WKing},
            {"BPawn", BPawn},
            {"BRook", BRook},
            {"BKnight", BKnight},
            {"BBishop", BBishop},
            {"BQueen", BQueen},
            {"BKing", BKing},
            {"NoPiece", NoPiece}
    };

    //Converts the pieces to the correct text
    static std::unordered_map<Pieces, std::string> piecesToString {
            {WPawn, "WPawn"},
            {WRook, "WRook"},
            {WKnight, "WKnight"},
            {WBishop, "WBishop"},
            {WQueen, "WQueen"},
            {WKing, "WKing"},
            {BPawn, "BPawn"},
            {BRook, "BRook"},
            {BKnight, "BKnight"},
            {BBishop, "BBishop"},
            {BQueen, "BQueen"},
            {BKing, "BKing"},
            {NoPiece, "NoPiece"}
    };
}
