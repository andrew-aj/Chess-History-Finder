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
#include <chrono>

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

        //Stores the initialization times
        std::string btime;
        std::string htime;
    public:
        //Initialize all of the sprites
        void initializePieceSelectionSprites();

        //Set the position and default texture for each piece on the board
        void initializeGameBoard();

        //Initialize the buttons and their textures and positions
        void initializeButtons();

        //Initialize the textures and positions for the highlighters
        void initializeHighlighters();

        //Initialize the textures and positions for the castling rights buttons
        void initializeCastleToggles();

        //Initialize the text and load in the font
        void initializeText();

        //Starts the GUI program
        void startProgramWindow();

        GUI() {

        }

        //Read the binary file
        void readFileBinary(const string &fileName);
    };
}
