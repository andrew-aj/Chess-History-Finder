#pragma once

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
        /*CONSTRUCTORS*/
        Board();

        /*BOARD INITIALIZERS*/
        void LoadTextures();
        void setBoard();

        void draw(sf::RenderWindow& window);
        void UpdateBlackWhite();
    };
}
