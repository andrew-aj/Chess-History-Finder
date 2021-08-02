#include "GUI.h"
#include "Heap.h"
#include "TextureManager.h"
#include <iostream>

namespace Chess {
    //Starts the GUI program
    void GUI::startProgramWindow() {
        //Creates the window and caps the fps to 60 so we don't explode our GPUs
        window.create(sf::VideoMode(1280, 720), "Chess History Finder!");
        window.setFramerateLimit(60);

        //Initializes all of the sprites
        initializePieceSelectionSprites();
        initializeGameBoard();
        initializeButtons();
        initializeHighlighters();
        initializeCastleToggles();
        initializeText();

        //Creates the background board
        sf::Sprite bg(TextureManager::GetTexture("bg"));
        board.clear();

        //Sets the variables for tracking previous clicks
        bool lastRightClicked = false;
        bool lastLeftClicked = false;

        //Sets if the current move is black
        bool blackToMove = false;

        //Sets castling rights
        bool topLeftCastleRights = true;
        bool topRightCastleRights = true;
        bool bottomLeftCastleRights = true;
        bool bottomRightCastleRights = true;

        //Opens file for outputting how long each algorithm takes
        std::ofstream f("time.txt");

        //Outputs the time taken for the initialization
        f << btime << std::endl;
        f << htime << std::endl;

        //Default piece selection
        string selectedPiece = "WPawn";

        //Sets the position of the selection
        highlighters["pieceSelection"].setPosition(pieceSelection[selectedPiece].getPosition().x + 40,
                                                   pieceSelection[selectedPiece].getPosition().y + 40);

        //Run the game while the window is open
        while (window.isOpen()) {
            sf::Event event;

            //Get the mouse position in the windw
            auto mouse_pos = sf::Mouse::getPosition(window);

            //Convert that to coordinates in the window
            auto translated_pos = window.mapPixelToCoords(mouse_pos);

            //Which mouse buttom was pressed
            bool rightClicked = sf::Mouse::isButtonPressed(sf::Mouse::Right);
            bool leftClicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);

            //Get the events from the window
            while (window.pollEvent(event)) {
                //Close the program if the window is closed
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            //Clear the window each frame
            window.clear();

            //Draw the board each frame
            window.draw(bg);


            //Check if left click was pressed and not being held down
            if (leftClicked && !lastLeftClicked && window.hasFocus()) {
                //Check if the mouse click was in the piece selection area and get the piece
                for (auto &iter : pieceSelection) {
                    if (iter.second.getGlobalBounds().contains(translated_pos)) {
                        highlighters["pieceSelection"].setPosition(iter.second.getPosition().x + 40,
                                                                   iter.second.getPosition().y + 40);
                        selectedPiece = iter.first;
                    }
                }

                //Loops through the gameboard to see which tile you clicked on
                for (unsigned int i = 0; i < gameBoard.size(); i++) {
                    // MAKE SURE YOU USE REFERENCE ITER WHEN CHANGING TEXTURE
                    auto iter = gameBoard.begin() + i;

                    if (iter->first.getGlobalBounds().contains(translated_pos) &&
                        iter->second != stringToPieces[selectedPiece]) {

                        iter->first.setTexture(TextureManager::GetTexture(selectedPiece));

                        iter->second = stringToPieces[selectedPiece];

                        board.addPiece(iter->second, i);
                    }
                }

                //Check all of the castle toggles and set them accordingly
                if (castleToggles["topLeft"].getGlobalBounds().contains(translated_pos)) {
                    topLeftCastleRights = !topLeftCastleRights;
                }

                if (castleToggles["topRight"].getGlobalBounds().contains(translated_pos)) {
                    topRightCastleRights = !topRightCastleRights;
                }

                if (castleToggles["bottomLeft"].getGlobalBounds().contains(translated_pos)) {
                    bottomLeftCastleRights = !bottomLeftCastleRights;
                }

                if (castleToggles["bottomRight"].getGlobalBounds().contains(translated_pos)) {
                    bottomRightCastleRights = !bottomRightCastleRights;
                }


                //Check the if the black to move button was pressed
                if (buttons["blackNext"].getGlobalBounds().contains(translated_pos)) {
                    blackToMove = true;
                }

                //Check if the white to move button was pressed
                if (buttons["whiteNext"].getGlobalBounds().contains(translated_pos)) {
                    blackToMove = false;
                }

                //Check if the clear button was pressed
                if (buttons["clear"].getGlobalBounds().contains(translated_pos)) {
                    //Clear the gameboard
                    board.clear();

                    //Reset the textures
                    for (auto &iter : gameBoard) {
                        iter.first.setTexture(TextureManager::GetTexture("transparentSquare"));
                        iter.second = NoPiece;
                    }

                    //Reset the castling rights
                    topLeftCastleRights = true;
                    topRightCastleRights = true;
                    bottomLeftCastleRights = true;
                    bottomRightCastleRights = true;

                    //Reset the recommended move
                    text.setString("");
                }

                //Check if the default button was pressed
                if (buttons["default"].getGlobalBounds().contains(translated_pos)) {
                    //Clear the board
                    board.clear();

                    //Set the board state to the default state
                    board.setDefault();

                    //Set the textures for each square
                    for (unsigned int i = 0; i < board.getBoard().size(); i++) {
                        gameBoard[i].second = board.getBoard()[i];
                        if (board.getBoard()[i] != NoPiece)
                            gameBoard[i].first.setTexture(
                                    TextureManager::GetTexture(piecesToString[board.getBoard()[i]]));
                        else
                            gameBoard[i].first.setTexture(TextureManager::GetTexture("transparentSquare"));
                    }

                    //Set the castling rights
                    topLeftCastleRights = true;
                    topRightCastleRights = true;
                    bottomLeftCastleRights = true;
                    bottomRightCastleRights = true;

                    //Reset the recommended move
                    text.setString("");
                }

                //Check if the submit button was pressed
                if (buttons["submit"].getGlobalBounds().contains(translated_pos)) {
                    //Calculate the new hash
                    board.calculateZobristHash(bottomRightCastleRights, bottomLeftCastleRights, topRightCastleRights,
                                               topLeftCastleRights, blackToMove);

                    //Get the hash
                    hash val = board.getHash();

                    //Time the amount taken for the tree to find the hash
                    auto start = std::chrono::high_resolution_clock::now();

                    //Search the tree for the hash
                    ZobristHash *zhBTree = tree.findHash(val);

                    auto end = std::chrono::high_resolution_clock::now();
                    //Calculate the duration
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
                    //Write the duration to the file
                    f << "B Tree search took " << duration.count() << " nanoseconds" << std::endl;

                    //Time the amount taken for the heap sorted array to find the hash
                    start = std::chrono::high_resolution_clock::now();
                    //Search the array using binary search
                    ZobristHash zhHeap = *std::lower_bound(sort.begin(), sort.end(), ZobristHash(val, 0));

                    end = std::chrono::high_resolution_clock::now();
                    //Calculate the duration
                    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
                    //Write the duration to the file
                    f << "Heap sorted array took " << duration.count() << " nanoseconds" << std::endl;

                    //Check if a hash was found
                    if (zhBTree != nullptr && zhHeap.Data == val) {
                        //Output it
                        std::cout << zhBTree->hashToMove() << std::endl;
                        text.setString(zhBTree->hashToMove());

                        //Check if tree matches what the array found
                        if (zhBTree->bestMove != zhHeap.bestMove) {
                            std::cout << "Error" << std::endl;
                            text.setString("Error!");
                        }
                    } else {
                        //Check if array also didn't find anything
                        if (zhHeap.Data == val) {
                            std::cout << "Error" << std::endl;
                            text.setString("Error!");
                        } else {
                            std::cout << "No Move" << std::endl;
                            text.setString("No Move");
                        }
                    }

                    //Set the position of the text
                    text.setOrigin(text.getGlobalBounds().width / 2, 0);
                }
            }

            //remove piece on right click
            if (rightClicked && !lastRightClicked && window.hasFocus()) {
                for (unsigned int i = 0; i < gameBoard.size(); i++) {
                    // MAKE SURE YOU USE REFERENCE ITER WHEN CHANGING TEXTURE
                    auto iter = gameBoard.begin() + i;
                    if (iter->first.getGlobalBounds().contains(translated_pos) && iter->second != NoPiece) {
                        iter->first.setTexture(TextureManager::GetTexture("transparentSquare"));
                        iter->second = NoPiece;
                        board.addPiece(iter->second, i);
                    }
                }
            }

            //Set the position of the highlighter for move selection
            if (blackToMove) {
                highlighters["nextColor"].setPosition(buttons["blackNext"].getPosition().x + 50,
                                                      buttons["blackNext"].getPosition().y + 20);
            } else {
                highlighters["nextColor"].setPosition(buttons["whiteNext"].getPosition().x + 50,
                                                      buttons["whiteNext"].getPosition().y + 20);
            }

            //The the status of the castling rights buttons
            if (topLeftCastleRights)
                castleToggles["topLeft"].setTexture(TextureManager::GetTexture("yesCastle"));
            else
                castleToggles["topLeft"].setTexture(TextureManager::GetTexture("noCastle"));

            if (topRightCastleRights)
                castleToggles["topRight"].setTexture(TextureManager::GetTexture("yesCastle"));
            else
                castleToggles["topRight"].setTexture(TextureManager::GetTexture("noCastle"));

            if (bottomLeftCastleRights)
                castleToggles["bottomLeft"].setTexture(TextureManager::GetTexture("yesCastle"));
            else
                castleToggles["bottomLeft"].setTexture(TextureManager::GetTexture("noCastle"));

            if (bottomRightCastleRights)
                castleToggles["bottomRight"].setTexture(TextureManager::GetTexture("yesCastle"));
            else
                castleToggles["bottomRight"].setTexture(TextureManager::GetTexture("noCastle"));


            //Draw all the objects
            for (const auto &iter : gameBoard)
                window.draw(iter.first);
            for (const auto &iter : pieceSelection)
                window.draw(iter.second);
            for (const auto &iter : buttons)
                window.draw(iter.second);
            for (const auto &iter : castleToggles)
                window.draw(iter.second);


            //Draw the highlighters
            for (const auto &iter : highlighters)
                window.draw(iter.second);

            //Draw the text
            window.draw(text);

            //Display the window
            window.display();

            //Set the mouse button booleans
            lastRightClicked = rightClicked;
            lastLeftClicked = leftClicked;
        }
    }

    //Read the binary file
    void GUI::readFileBinary(const string &fileName) {
        std::cout << "GUI starting..." << std::endl;

        //Open the file
        std::fstream file(fileName, std::ios::binary | std::ios::in);
        uint64_t totalSuccessfulHashes = 0;

        //Read in the random numbers for zobrist hash calculations
        file.read((char *) ZobristHash::randNums.data(), 781 * 8);

        //Get the size of the number of hashes
        file.read((char *) &totalSuccessfulHashes, 8);

        //Resize the array to be sorted
        sort.resize(totalSuccessfulHashes);

        //Read in the data
        for (uint64_t i = 0; i < totalSuccessfulHashes; i++) {
            file.read((char *) &sort[i].Data, 8);
            file.read((char *) &sort[i].bestMove, 2);
        }

        file.close();

        //Time the total amount taken for inserting all the data into the b tree
        auto start = std::chrono::high_resolution_clock::now();

        //Insert all the data into the b tree
        for (unsigned int i = 0; i < sort.size(); i++) {
            tree.insertHash(sort[i].Data, sort[i].bestMove);
        }

        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        //Set the duration
        btime = "B Tree insertions took " + std::to_string(duration.count()) + " milliseconds";

        //The the amount taken to build the tree based heap and then use heap sort
        start = std::chrono::high_resolution_clock::now();

        Heap::HeapSort(sort);

        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        //Write the duration to file
        htime = "Heap sort took " + std::to_string(duration.count()) + " milliseconds";
    }

    //Initialize all of the sprites
    void GUI::initializePieceSelectionSprites() {
        sf::Sprite WPawn(TextureManager::GetTexture("WPawn"));
        WPawn.setPosition(60, 120);
        pieceSelection["WPawn"] = WPawn;

        sf::Sprite WRook(TextureManager::GetTexture("WRook"));
        WRook.setPosition(60, 200);
        pieceSelection["WRook"] = WRook;

        sf::Sprite WKnight(TextureManager::GetTexture("WKnight"));
        WKnight.setPosition(60, 280);
        pieceSelection["WKnight"] = WKnight;

        sf::Sprite WBishop(TextureManager::GetTexture("WBishop"));
        WBishop.setPosition(60, 360);
        pieceSelection["WBishop"] = WBishop;

        sf::Sprite WQueen(TextureManager::GetTexture("WQueen"));
        WQueen.setPosition(60, 440);
        pieceSelection["WQueen"] = WQueen;

        sf::Sprite WKing(TextureManager::GetTexture("WKing"));
        WKing.setPosition(60, 520);
        pieceSelection["WKing"] = WKing;

        sf::Sprite BPawn(TextureManager::GetTexture("BPawn"));
        BPawn.setPosition(140, 120);
        pieceSelection["BPawn"] = BPawn;

        sf::Sprite BRook(TextureManager::GetTexture("BRook"));
        BRook.setPosition(140, 200);
        pieceSelection["BRook"] = BRook;

        sf::Sprite BKnight(TextureManager::GetTexture("BKnight"));
        BKnight.setPosition(140, 280);
        pieceSelection["BKnight"] = BKnight;

        sf::Sprite BBishop(TextureManager::GetTexture("BBishop"));
        BBishop.setPosition(140, 360);
        pieceSelection["BBishop"] = BBishop;

        sf::Sprite BQueen(TextureManager::GetTexture("BQueen"));
        BQueen.setPosition(140, 440);
        pieceSelection["BQueen"] = BQueen;

        sf::Sprite BKing(TextureManager::GetTexture("BKing"));
        BKing.setPosition(140, 520);
        pieceSelection["BKing"] = BKing;
    }

    //Set the position and default texture for each piece on the board
    void GUI::initializeGameBoard() {
        for (unsigned int i = 0; i < 8; i++) {
            for (unsigned int j = 0; j < 8; j++) {
                gameBoard.emplace_back(TextureManager::GetTexture("transparentSquare"), NoPiece);
                gameBoard[j + (8 * i)].first.setPosition(320 + (80 * j), 600 - (80 * i));
            }
        }

    }

    //Initialize the buttons and their textures and positions
    void GUI::initializeButtons() {
        sf::Sprite _clear(TextureManager::GetTexture("transparentButton"));
        _clear.setPosition(1036, 40);
        buttons["clear"] = _clear;

        sf::Sprite _default(TextureManager::GetTexture("transparentButton"));
        _default.setPosition(1144, 40);
        buttons["default"] = _default;

        sf::Sprite whiteNext(TextureManager::GetTexture("transparentButton"));
        whiteNext.setPosition(1036, 240);
        buttons["whiteNext"] = whiteNext;

        sf::Sprite blackNext(TextureManager::GetTexture("transparentButton"));
        blackNext.setPosition(1144, 240);
        buttons["blackNext"] = blackNext;

        sf::Sprite submit(TextureManager::GetTexture("transparentButton"));
        submit.setPosition(1090, 400);
        buttons["submit"] = submit;

    }

    //Initialize the textures and positions for the highlighters
    void GUI::initializeHighlighters() {
        highlighters["pieceSelection"] = sf::Sprite(TextureManager::GetTexture("highlightPiece"));
        highlighters["pieceSelection"].setOrigin(40, 40);

        highlighters["nextColor"] = sf::Sprite(TextureManager::GetTexture("highlightButton"));
        highlighters["nextColor"].setOrigin(53, 23);
    }

    //Initialize the textures and positions for the castling rights buttons
    void GUI::initializeCastleToggles() {
        sf::Sprite topLeft(TextureManager::GetTexture("yesCastle"));
        topLeft.setPosition(280, 0);
        castleToggles["topLeft"] = topLeft;

        sf::Sprite topRight(TextureManager::GetTexture("yesCastle"));
        topRight.setPosition(963, 0);
        castleToggles["topRight"] = topRight;

        sf::Sprite bottomLeft(TextureManager::GetTexture("yesCastle"));
        bottomLeft.setPosition(280, 683);
        castleToggles["bottomLeft"] = bottomLeft;

        sf::Sprite bottomRight(TextureManager::GetTexture("yesCastle"));
        bottomRight.setPosition(963, 683);
        castleToggles["bottomRight"] = bottomRight;
    }

    //Initialize the text and load in the font
    void GUI::initializeText() {
        font.loadFromFile("fonts/TimesNewerRoman-Bold.otf");

        text.setFont(font);

        text.setCharacterSize(44);

        text.setFillColor(sf::Color(230, 198, 156));

        text.setOrigin(text.getGlobalBounds().width / 2, 0);

        text.setPosition(1140, 580 - 32);
    }
}
