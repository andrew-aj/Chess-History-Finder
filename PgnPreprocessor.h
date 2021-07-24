#pragma once

#include "Defines.h"
#include "NextMove.h"
#include <string>
#include <vector>
#include <map>
#include <set>
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::multiset;

namespace Chess {
    class PgnPreprocessor {
    private:
        struct BoardState {
            hash data;
            NextMove next;
        };

        map<hash, multiset<NextMove>> hashStorage; // Multiset keeps track of how many times a NextMove occurs.
    public:
        void readFile(const string& fileName);
    };
}