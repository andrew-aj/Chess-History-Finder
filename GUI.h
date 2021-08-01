#pragma once

#include "TextureManager.h"
#include "Defines.h"
#include "BTree.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <fstream>

using std::map;

namespace Chess {
    class GUI {
    private:
        sf::RenderWindow window;
        map<string, sf::Sprite> sprites;

        BTree tree;
        std::vector<ZobristHash> sort;
    public:
        void startProgramWindow();

        GUI() {

        }

        void readFileBinary(const string &fileName);
    };
}
