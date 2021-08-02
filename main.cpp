#include "GUI.h"
#include "PgnPreprocessor.h"

using Chess::ZobristHash;

int main(int argc, const char *argv[]) {
    //Start gui with 1 argument which is the preprocessed file to load in
    //Start preprocess with 2 arguments which is the raw file and the file to store the processed information in

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
