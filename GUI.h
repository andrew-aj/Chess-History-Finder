#pragma once

#include "TextureManager.h"
#include "Defines.h"
#include <SFML/Graphics.hpp>
#include <map>

using std::map;

namespace Chess {
    class GUI {
    private:
        sf::RenderWindow window;
        map<string, sf::Sprite> sprites;
    public:
        void startProgramWindow();

        GUI() {

        }
    };
}
