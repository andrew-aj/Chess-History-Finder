//
// Created by hicke on 7/26/2021.
//

#ifndef CHESSHISTORYFINDER_BOARD_H
#define CHESSHISTORYFINDER_BOARD_H

#include "Defines.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>

using sf::Sprite;
namespace Chess{
    class Board{
        TextureManager textureManager;

        Sprite Pawn;
        Sprite Rook;
        Sprite Bishop;
        Sprite Queen;
        Sprite King;
        Sprite Knight;

    public:
        /*BOARD INITIALIZERS*/
        void LoadTextures();
        void setBoard();

        void draw(sf::RenderWindow& window);
        void UpdateBlackWhite()
    };
}


#endif //CHESSHISTORYFINDER_BOARD_H
