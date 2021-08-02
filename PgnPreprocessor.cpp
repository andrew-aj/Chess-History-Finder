#include "PgnPreprocessor.h"
#include "BoardStorage.h"
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
        cout << "Started reading file! This may take a while..." << endl;

        while (!file.eof()) {
            getline(file, currLine);

            // Checks if current line is the Result line for a game.
            if (currLine.length() >= 7 && currLine.substr(1, 6) == "Result") {
                result = currLine;

                while (!file.eof()) {
                    getline(file, currLine);

                    // Any line starting with a '1' is the beginning of a moveText.
                    if (currLine[0] == '1') {
                        moveText = currLine;
                        break;
                    }
                }

                // Simulates the game and stores it in hashStorage map.
                simulateGame(moveText, result);
            }

            // Requires around 10 gb of ram to read in 50,000,000 hashes.
            if (hashStorage.size() > 50000000) {
                break;
            }
        }

        cout << "Done reading file!" << endl;
    }

    void PgnPreprocessor::simulateGame(const string &moveText, const string &result) {
        BoardStorage game;

        // 11th character of result string is either 0, 1, or 2 depending on the result of the game.
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
            default: // If any other result is reported, it is discarded.
                return;
        }


        string moves;
        stringstream ss;
        ss << moveText;


        while (!ss.eof()) {
            string whiteMove;
            string blackMove;

            getline(ss, moves, '.'); // Skips each move number, '1.', in the moveText.
            if (ss.eof()) // If white wins the game, this makes sure the results aren't considered as a black move.
                break;

            ss >> whiteMove;
            if (whiteMove[0] >= '0' && whiteMove[0] <= '9') {
                // Results for game were read and the game has finished.
                break;
            } else {
                unordered_map<NextMove, unsigned long long> &currMoveStore = hashStorage[game.getHash()];

                // Generates a nextMove from pgn notation, and adds to the count of that move in currMoveStore.
                NextMove nextWhiteMove = game.generateMove(whiteMove, false);
                if (currMoveStore.find(nextWhiteMove) == currMoveStore.end())
                    currMoveStore[nextWhiteMove] = 1;
                else
                    currMoveStore[nextWhiteMove]++;

                game.applyMove(nextWhiteMove);
            }

            ss >> blackMove;
            if (blackMove[0] >= '0' && blackMove[0] <= '9') {
                // Results for game were read and the game has finished.
                break;
            } else {
                unordered_map<NextMove, unsigned long long> &currMoveStore = hashStorage[game.getHash()];

                // Generates a nextMove from pgn notation, and adds to the count of that move in currMoveStore.
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
        cout << "Calculating next best moves..." << endl;

        for (auto hashIter : hashStorage) {
            vector<pair<unsigned int, NextMove>> whiteNextMoves;
            vector<pair<unsigned int, NextMove>> blackNextMoves;

            for (auto moveIter : hashIter.second) {
                bool isBlackMove = ((moveIter.first >> 14) & 0b1);
                bool isWinningMove = ((moveIter.first >> 15) & 0b1);

                // Makes sure that the move led to a win or a tie before considering it for the the next best move.
                if (!isBlackMove && isWinningMove)
                    whiteNextMoves.emplace_back(moveIter.second, moveIter.first);
                if (isBlackMove && isWinningMove)
                    blackNextMoves.emplace_back(moveIter.second, moveIter.first);
            }

            // Whichever winning move occurred most often is the next best move.
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

        for (const auto& iter : calculatedStorage) {
            file << iter.first << ": " << iter.second << endl;
        }
    }

    void PgnPreprocessor::writeFileBinary(const string &fileName) {
        cout << "Writing binary file..." << endl;

        ofstream file(fileName, ios::binary);
        uint64_t totalSuccessfulHashes = calculatedStorage.size();

        // Table of randNums is stored first, then number of hashes, then each hash and the respective best move.
        file.write((char *) ZobristHash::randNums.data(), 781 * 8);
        file.write((char *) &totalSuccessfulHashes, 8);

        for (auto iter : calculatedStorage) {
            file.write((char *) &iter.first, 8);
            file.write((char *) &iter.second, 2);
        }

        cout << "Done writing binary file!" << endl;
    }

    ZobristHash *PgnPreprocessor::readFileBinary(const string &fileName, BTree &bTree, uint64_t &size) {
        ifstream file(fileName, ios::binary);
        uint64_t totalSuccessfulHashes = 0;

        file.read((char *) ZobristHash::randNums.data(), 781 * 8);
        file.read((char *) &totalSuccessfulHashes, 8);

        size = totalSuccessfulHashes;

        ZobristHash *arr = new ZobristHash[size];

        // Stores location of the file to go back to after filling the BTree.
        int loc = file.tellg();

        hash tempHash;
        NextMove tempMove;

        // Filling the BTree.
        for (uint64_t i = 0; i < totalSuccessfulHashes; i++) {
            file.read((char *) &tempHash, 8);
            file.read((char *) &tempMove, 2);
            bTree.insertHash(tempHash, tempMove);
        }

        file.seekg(loc);

        // Filling an array to later be sorted.
        for (uint64_t i = 0; i < totalSuccessfulHashes; i++) {
            file.read((char *) &arr[i].Data, 8);
            file.read((char *) &arr[i].bestMove, 2);
        }

        return arr;
    }
}