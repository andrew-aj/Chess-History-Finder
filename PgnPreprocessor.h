#pragma once

#include "Defines.h"
#include "NextMove.h"
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
        unordered_map<hash, unordered_map<NextMove, unsigned int>> hashStorage; // Inner map keeps track of how many times a NextMove occurs.
        unordered_map<hash, set<NextMove>> calculatedStorage; // Top three moves (at most) stored in set after calculations.
    public:
        void readFile(const string& fileName);
        void simulateGame(const string& moveText, const string& results);
        void calculateNextBestMoves();
        void writeFile(const string& fileName);
    };
}