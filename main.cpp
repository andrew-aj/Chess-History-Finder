#include <iostream>
#include "ChessHistoryFinder.h"
#include "PgnPreprocessor.h"

int main() {
//    Chess::ChessHistoryFinder program;
//    program.startProgramWindow();
    Chess::PgnPreprocessor program;
    program.readFile("test.pgn");
    program.writeFile("out.effective");
}
