#include <iostream>
#include "ChessHistoryFinder.h"
#include "PgnPreprocessor.h"
#include "ZobristHash.h"

int main() {

    /* Code to start gui */
//    Chess::ChessHistoryFinder gui;
//    gui.startProgramWindow();

    /* Code to start preprocessor */
    Chess::ZobristHash::generateRandomNumbers();
    Chess::PgnPreprocessor preprocessor;
    preprocessor.readFile("test.pgn");
    preprocessor.calculateNextBestMoves();
    preprocessor.writeFile("out.effective");
}
