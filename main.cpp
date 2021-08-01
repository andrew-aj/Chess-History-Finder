#include <iostream>
#include "ChessHistoryFinder.h"
#include "PgnPreprocessor.h"
#include "ZobristHash.h"
#include "BoardStorage.h"

int main() {
//    Chess::ChessHistoryFinder program;
//    program.startProgramWindow();
    Chess::PgnPreprocessor program;
    Chess::ZobristHash::generateRandomNumbers();
    program.readFile("out.pgn");
    program.writeFile("out.effective");
}
