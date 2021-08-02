#include "GUI.h"
#include "TextureManager.h"
#include <iostream>

namespace Chess {
    void GUI::startProgramWindow() {
        window.create(sf::VideoMode(1280, 720), "Chess History Finder!");
        window.setFramerateLimit(60);
        initializePieceSelectionSprites();
        initializeGameBoard();
        initializeButtons();
        initializeHighlighters();
        initializeCastleToggles();

        sf::Sprite bg(TextureManager::GetTexture("bg"));
        board.clear();

        bool lastRightClicked = false;
        bool lastLeftClicked = false;

        bool blackToMove = false;
        bool topLeftCastleRights = true;
        bool topRightCastleRights = true;
        bool bottomLeftCastleRights = true;
        bool bottomRightCastleRights = true;

        string selectedPiece = "WPawn";
        highlighters["pieceSelection"].setPosition(pieceSelection[selectedPiece].getPosition().x + 40, pieceSelection[selectedPiece].getPosition().y + 40);

        while (window.isOpen()) {
            sf::Event event;
            auto mouse_pos = sf::Mouse::getPosition(window);
            auto translated_pos = window.mapPixelToCoords(mouse_pos);
            bool rightClicked = sf::Mouse::isButtonPressed(sf::Mouse::Right);
            bool leftClicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            window.draw(bg);



            if (leftClicked && !lastLeftClicked) {
                for (auto &iter : pieceSelection) {
                    if (iter.second.getGlobalBounds().contains(translated_pos)) {
                        highlighters["pieceSelection"].setPosition(iter.second.getPosition().x + 40, iter.second.getPosition().y + 40);
                        selectedPiece = iter.first;
                    }
                }

                for (unsigned int i = 0; i < gameBoard.size(); i++) { // MAKE SURE YOU USE REFERENCE ITER WHEN CHANGING TEXTURE
                    auto iter = gameBoard.begin() + i;
                    if (iter->first.getGlobalBounds().contains(translated_pos) && iter->second != stringToPieces[selectedPiece]) {
                        iter->first.setTexture(TextureManager::GetTexture(selectedPiece));
                        iter->second = stringToPieces[selectedPiece];
                        board.addPiece(iter->second, i);
                    }
                }

                if (castleToggles["topLeft"].getGlobalBounds().contains(translated_pos)) {
                    topLeftCastleRights = !topLeftCastleRights;
                    if (topLeftCastleRights)
                        castleToggles["topLeft"].setTexture(TextureManager::GetTexture("yesCastle"));
                    else
                        castleToggles["topLeft"].setTexture(TextureManager::GetTexture("noCastle"));
                }
                if (castleToggles["topRight"].getGlobalBounds().contains(translated_pos)) {
                    topRightCastleRights = !topRightCastleRights;
                    if (topRightCastleRights)
                        castleToggles["topRight"].setTexture(TextureManager::GetTexture("yesCastle"));
                    else
                        castleToggles["topRight"].setTexture(TextureManager::GetTexture("noCastle"));
                }
                if (castleToggles["bottomLeft"].getGlobalBounds().contains(translated_pos)) {
                    bottomLeftCastleRights = !bottomLeftCastleRights;
                    if (bottomLeftCastleRights)
                        castleToggles["bottomLeft"].setTexture(TextureManager::GetTexture("yesCastle"));
                    else
                        castleToggles["bottomLeft"].setTexture(TextureManager::GetTexture("noCastle"));
                }
                if (castleToggles["bottomRight"].getGlobalBounds().contains(translated_pos)) {
                    bottomRightCastleRights = !bottomRightCastleRights;
                    if (bottomRightCastleRights)
                        castleToggles["bottomRight"].setTexture(TextureManager::GetTexture("yesCastle"));
                    else
                        castleToggles["bottomRight"].setTexture(TextureManager::GetTexture("noCastle"));
                }


                if (buttons["blackNext"].getGlobalBounds().contains(translated_pos)) {
                    blackToMove = true;
                }

                if (buttons["whiteNext"].getGlobalBounds().contains(translated_pos)) {
                    blackToMove = false;
                }

                if (buttons["clear"].getGlobalBounds().contains(translated_pos)) {
                    board.clear();
                    for (auto &iter : gameBoard) {
                        iter.first.setTexture(TextureManager::GetTexture("transparentSquare"));
                        iter.second = NoPiece;
                    }
                }

                if (buttons["default"].getGlobalBounds().contains(translated_pos)) {
                    board.clear();
                    board.setDefault();
                    for (unsigned int i = 0; i < board.getBoard().size(); i++) {
                        gameBoard[i].second = board.getBoard()[i];
                        if (board.getBoard()[i] != NoPiece)
                            gameBoard[i].first.setTexture(TextureManager::GetTexture(piecesToString[board.getBoard()[i]]));
                        else
                            gameBoard[i].first.setTexture(TextureManager::GetTexture("transparentSquare"));
                    }
                }

                if (buttons["submit"].getGlobalBounds().contains(translated_pos)) {
                    board.calculateZobristHash(bottomRightCastleRights, bottomLeftCastleRights, topRightCastleRights, topLeftCastleRights, blackToMove);
                    hash val = board.getHash();


                }
            }

            if (rightClicked && !lastRightClicked) {
                for (unsigned int i = 0; i < gameBoard.size(); i++) { // MAKE SURE YOU USE REFERENCE ITER WHEN CHANGING TEXTURE
                    auto iter = gameBoard.begin() + i;
                    if (iter->first.getGlobalBounds().contains(translated_pos) && iter->second != NoPiece) {
                        iter->first.setTexture(TextureManager::GetTexture("transparentSquare"));
                        iter->second = NoPiece;
                        board.addPiece(iter->second, i);
                    }
                }
            }

            if (blackToMove) {
                highlighters["nextColor"].setPosition(buttons["blackNext"].getPosition().x + 50, buttons["blackNext"].getPosition().y + 20);
            } else {
                highlighters["nextColor"].setPosition(buttons["whiteNext"].getPosition().x + 50, buttons["whiteNext"].getPosition().y + 20);
            }


            for (const auto& iter : gameBoard)
                window.draw(iter.first);
            for (const auto& iter : pieceSelection)
                window.draw(iter.second);
            for (const auto& iter : buttons)
                window.draw(iter.second);
            for (const auto& iter : castleToggles)
                window.draw(iter.second);


            for (const auto& iter : highlighters)
                window.draw(iter.second);


            window.display();

            lastRightClicked = rightClicked;
            lastLeftClicked = leftClicked;
        }
    }

    void GUI::readFileBinary(const string &fileName) {
        std::ifstream file(fileName, std::ios::binary);
        uint64_t totalSuccessfulHashes = 0;

        file.read((char*)ZobristHash::randNums.data(), 781*8);
        file.read((char *)&totalSuccessfulHashes, 8);

        sort.reserve(totalSuccessfulHashes);

        for(uint64_t i = 0; i < totalSuccessfulHashes; i++){
            //std::cout << i << std::endl;
            file.read((char*)&sort[i].Data, 8);
            file.read((char*)&sort[i].bestMove, 2);
            tree.insertHash(sort[i].Data, sort[i].bestMove);
        }
    }

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

    void GUI::initializeGameBoard() {
        for (unsigned int i = 0; i < 8; i++) {
            for (unsigned int j = 0; j < 8; j++) {
                gameBoard.emplace_back(TextureManager::GetTexture("transparentSquare"), NoPiece);
                gameBoard[j + (8 * i)].first.setPosition(320 + (80 * j), 600 - (80 * i));
            }
        }

    }

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

    void GUI::initializeHighlighters() {
        highlighters["pieceSelection"] = sf::Sprite(TextureManager::GetTexture("highlightPiece"));
        highlighters["pieceSelection"].setOrigin(40, 40);
        highlighters["nextColor"] = sf::Sprite(TextureManager::GetTexture("highlightButton"));
        highlighters["nextColor"].setOrigin(53, 23);
    }

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
}
