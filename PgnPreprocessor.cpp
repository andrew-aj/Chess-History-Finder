#include "PgnPreprocessor.h"
#include "BoardStorage.h"
#include "NextMove.h"
#include "ZobristHash.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <algorithm>
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::getline;
using std::stringstream;
using std::pair;
using std::priority_queue;
using std::max_element;
using std::ios;

namespace Chess {
    void PgnPreprocessor::readFile(const string &fileName) {
        string currLine;
        string result;
        string moveText;

        ifstream file(fileName);

        while (!file.eof()) {
            getline(file, currLine);

            if (currLine.length() >= 7 && currLine.substr(1, 6) == "Result") {
                result = currLine;

                while (!file.eof()) {
                    getline(file, currLine);

                    if (currLine[0] == '1') {
                        moveText = currLine;
                        break;
                    }
                }

                simulateGame(moveText, result);
                cout << "simulate complete." << endl;
            }
        }
    }

    void PgnPreprocessor::simulateGame(const string &moveText, const string& result) {
//        if (moveText.find("eval"))
//            return;

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


        while(!ss.eof()) {
            string whiteMove;
            string blackMove;

            getline(ss, moves, '.'); // skips the first '.' in the moveText
            if (ss.eof()) // If white wins the game, this makes sure the results aren't considered as a black move.
                break;

            ss >> whiteMove;
            if (whiteMove[0] >= '0' && whiteMove[0] <= '9') {
                // the game has finished
                break;
            } else {
                unordered_map<NextMove, unsigned int>& currMoveStore = hashStorage[game.getHash()];

                NextMove nextWhiteMove = game.generateMove(whiteMove, false);
                if (currMoveStore.find(nextWhiteMove) == currMoveStore.end())
                    currMoveStore[nextWhiteMove] = 1;
                else
                    currMoveStore[nextWhiteMove]++;

                game.applyMove(nextWhiteMove);
            }

            ss >> blackMove;
            if (blackMove[0] >= '0' && blackMove[0] <= '9') {
                // the game has finished
                break;
            } else {
                unordered_map<NextMove, unsigned int>& currMoveStore = hashStorage[game.getHash()];

                NextMove nextBlackMove = game.generateMove(blackMove, true);
                if (currMoveStore.find(nextBlackMove) == currMoveStore.end())
                    currMoveStore[nextBlackMove] = 1;
                else
                    currMoveStore[nextBlackMove]++;

                game.applyMove(nextBlackMove);
            }

        }
    }

    void PgnPreprocessor::calculateNextBestMoves() {
        for (auto hashIter : hashStorage) {
            vector<pair<unsigned int, NextMove>> whiteNextMoves;
            vector<pair<unsigned int, NextMove>> blackNextMoves;

            for (auto moveIter : hashIter.second) {
                // TODO confirm with cheezy that this isBlackMove check works
                bool isBlackMove = ((moveIter.first >> 14) & 0b1);
                bool isWinningMove = ((moveIter.first >> 15) & 0b1);

                if (!isBlackMove && isWinningMove)
                    whiteNextMoves.emplace_back(moveIter.second, moveIter.first);
                if (isBlackMove && isWinningMove)
                    blackNextMoves.emplace_back(moveIter.second, moveIter.first);
            }

            if (!whiteNextMoves.empty()) {
                NextMove bestWhiteMove = max_element(whiteNextMoves.begin(), whiteNextMoves.end())->second;
                calculatedStorage[hashIter.first] = bestWhiteMove;
            }
            if (!blackNextMoves.empty()) {
                NextMove bestBlackMove = max_element(blackNextMoves.begin(), blackNextMoves.end())->second;
                calculatedStorage[hashIter.first] = bestBlackMove;
            }
        }
    }

    void PgnPreprocessor::writeFile(const string &fileName) {
        ofstream file(fileName);

        for (auto iter = calculatedStorage.begin(); iter != calculatedStorage.end(); iter++) {
            file << iter->first << ": " << iter->second << endl;
        }
    }

    void PgnPreprocessor::writeFileBinary(const string &fileName) {
        ofstream file(fileName, ios::binary);
        uint64_t totalSuccessfulHashes = calculatedStorage.size();

        file.write((char*)ZobristHash::randNums.data(), 781 * 8);
        file.write((char*)&totalSuccessfulHashes, 8);

        for (auto iter : calculatedStorage) {
            file.write((char*)&iter.first, 8);
            file.write((char*)&iter.second, 2);
        }
    }

}