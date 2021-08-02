#pragma once

#include "Defines.h"
#include "BTree.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::multiset;
using std::set;

namespace Chess {
    class PgnPreprocessor {
    private:
        // Inner map keeps track of how many times a NextMove occurs.
        unordered_map<hash, unordered_map<NextMove, unsigned long long>> hashStorage;
        // Stores next best move for board state.
        unordered_map<hash, NextMove> calculatedStorage;
    public:
        // Reads passed file into hashStorage map.
        void readFile(const string &fileName);

        // Simulates a game from passed moveText and passed results, stores each generated hash into hashStorage map.
        void simulateGame(const string &moveText, const string &results);

        // Calculates best moves from hashStorage map and stores them in calculatedStorage map.
        void calculateNextBestMoves();

        // Writes a readable file (mainly for testing as it is slow and takes up a lot of space).
        void writeFile(const string &fileName);

        // Writes a binary file to be read by the GUI program later.
        void writeFileBinary(const string &fileName);

        // Reads in binary file and returns an array of ZobristHash objects.
        static ZobristHash *readFileBinary(const string &fileName, BTree &bTree, uint64_t &size);
    };
}