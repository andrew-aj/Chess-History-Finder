#include "ZobristHash.h"

namespace Chess {
    std::array<hash, 781> ZobristHash::randNums;

    bool ZobristHash::operator==(const ZobristHash &rhs) const {
        return Data == rhs.Data;
    }

    bool ZobristHash::operator<(const ZobristHash &rhs) const {
        return Data < rhs.Data;
    }

    bool ZobristHash::operator>(const ZobristHash &rhs) const {
        return rhs < *this;
    }

    bool ZobristHash::operator<=(const ZobristHash &rhs) const {
        return !(rhs < *this);
    }

    bool ZobristHash::operator>=(const ZobristHash &rhs) const {
        return !(*this < rhs);
    }

    ZobristHash::ZobristHash() {

    }

    //Generate the random numbers for the zobrist hash based on this from cppreference
    //https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    void ZobristHash::generateRandomNumbers() {
        if (!initialized()){
            std::random_device rd;

            std::default_random_engine generator(rd());

            std::uniform_int_distribution<uint64_t> distribution(0, 0xFFFFFFFFFFFFFFFF);

            //Uses the std::generate function from algorithm.h
            std::generate(randNums.begin(), randNums.end(), [&](){return distribution(generator);});
        }
    }

    //Constructor
    ZobristHash::ZobristHash(hash data, NextMove bestMove) {
        this->Data = data;
        this->bestMove = bestMove;
    }

    //Returns if it's initialized if all values aren't 0
    bool ZobristHash::initialized() {
        return std::count(randNums.begin(), randNums.end(), 0) != 781;
    }

    //Converts a given NextMove binary number to a string in hyphenated notation
    std::string ZobristHash::hashToMove() {
        std::string hyphenatedMove;

        //Gets the 6 bits from the move
        uint8_t from = (bestMove >> 8) & 0b111111;

        //Converts it to the correct char
        hyphenatedMove += ((from) % 8) + 'a';
        hyphenatedMove += ((from) / 8) + '1';

        uint8_t to = (bestMove >> 2) & 0b111111;
        hyphenatedMove += '-';
        hyphenatedMove += ((to) % 8) + 'a';
        hyphenatedMove += ((to) / 8) + '1';

        return hyphenatedMove;
    }
}