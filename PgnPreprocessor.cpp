#include "PgnPreprocessor.h"
#include "BoardStorage.h"
#include "NextMove.h"
#include <fstream>
#include <iostream>
#include <sstream>
using std::ifstream;
using std::cout;
using std::endl;
using std::getline;
using std::stringstream;

namespace Chess {
    void PgnPreprocessor::readFile(const string &fileName) {
        string currLine;
        string result;
        string moveText;

        ifstream file(fileName);

        while (true) {
            getline(file, currLine);

            if (file.eof())
                break;

            if (currLine.substr(1, 6) == "Result") {
                result = currLine;

                while (!file.eof()) {
                    getline(file, currLine);

                    if (currLine[0] == '1') {
                        moveText = currLine;
                        break;
                    }
                }

                simulateGame(moveText, result);
            }
        }
    }

    void PgnPreprocessor::simulateGame(const string &moveText, const string& result) {
        if (moveText.find("eval"))
            return;

        BoardStorage game;

        switch (result[11]) {
            case '0':
                game.setWhiteWinner();
                break;
            case '1':
                game.setBlackWinner();
                break;
            case '2':
                game.setDraw();
                break;
            default:
                cout << "somethings wrong..." << endl;
        }


        string moves;
        stringstream ss;
        ss << moveText;


        while(true) {
            string whiteMove;
            string blackMove;
            NextMove white {};
            NextMove black {};

            getline(ss, moves, '.'); // skips the first '.' in the moveText

            ss >> whiteMove;
            if (whiteMove[0] >= '0' && whiteMove[0] <= '9') {
                // the game has finished
                break;
            } else {
                white = game.generateMove(whiteMove, false);
                hashStorage[game.getHash()].insert(white);

                game.applyMove(white);
            }

            ss >> blackMove;
            if (blackMove[0] >= '0' && blackMove[0] <= '9') {
                // the game has finished
                break;
            } else {
                black = game.generateMove(blackMove, true);
                hashStorage[game.getHash()].insert(black);

                game.applyMove(black);
            }

        }
    }

    void PgnPreprocessor::writeFile(const string &fileName) {
        // TODO
    }
}