#include <iostream>
#include "GUI.h"
#include "PgnPreprocessor.h"
#include "ZobristHash.h"
#include "Heap.h"
using Chess::ZobristHash;

int main() {

    /* Code to start gui */
    Chess::GUI gui;
    gui.readFileBinary("outbin.effective");
    gui.startProgramWindow();

    /* Code to start preprocessor */
//    Chess::ZobristHash::generateRandomNumbers();
//    Chess::PgnPreprocessor preprocessor;
//    preprocessor.readFile("test_small.pgn");
//    preprocessor.calculateNextBestMoves();
//    preprocessor.writeFileBinary("outbin.effective");

}
