#pragma once

#include "TextureManager.h"
#include "Defines.h"
#include "BTree.h"
#include "ChessBinaryRep.h"
#include "BoardStorage.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <fstream>
#include <vector>

using std::map;
using std::vector;
using std::pair;

namespace Chess {
    class GUI {
    private:
        sf::RenderWindow window;
        map<string, sf::Sprite> pieceSelection;
        map<string, sf::Sprite> buttons;
        map<string, sf::Sprite> castleToggles;
        vector<pair<sf::Sprite, Pieces>> gameBoard;
        map<string, sf::Sprite> highlighters;
        sf::Font font;
        sf::Text text;

        BoardStorage board;
        BTree tree;
        std::vector<ZobristHash> sort;
    public:
        void initializePieceSelectionSprites();
        void initializeGameBoard();
        void initializeButtons();
        void initializeHighlighters();
        void initializeCastleToggles();
        void initializeText();
        void startProgramWindow();

        GUI() {

        }

        void readFileBinary(const string &fileName);
    };
}
