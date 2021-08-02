#include "GUI.h"
#include "PgnPreprocessor.h"

using Chess::ZobristHash;

int main(int argc, const char *argv[]) {
    // Start GUI with 1 argument: the preprocessed binary file to load in.
    // Start preprocessor with 2 arguments: The raw pgn file and the preprecessed binary file to be outputted.

    /* Code to start GUI. */
    if (argc == 2) {
        Chess::GUI gui;
        gui.readFileBinary(argv[1]);
        gui.startProgramWindow();
    } else if (argc == 3) {
    /* Code to start preprocessor. */
        Chess::ZobristHash::generateRandomNumbers();
        Chess::PgnPreprocessor preprocessor;
        preprocessor.readFile(argv[1]);
        preprocessor.calculateNextBestMoves();
        preprocessor.writeFileBinary(argv[2]);
    }


}
