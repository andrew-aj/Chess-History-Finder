#include <iostream>
#include "GUI.h"
#include "PgnPreprocessor.h"
#include "ZobristHash.h"
#include "Heap.h"

using Chess::ZobristHash;

int main(int argc, const char *argv[]) {

    /* Code to start gui */
    if (argc == 2) {
        Chess::GUI gui;
        gui.readFileBinary(argv[1]);
        gui.startProgramWindow();
    } else if (argc == 3) {
        /* Code to start preprocessor */
        Chess::ZobristHash::generateRandomNumbers();
        Chess::PgnPreprocessor preprocessor;
        preprocessor.readFile(argv[1]);
        preprocessor.calculateNextBestMoves();
        preprocessor.writeFileBinary(argv[2]);
    }


}
