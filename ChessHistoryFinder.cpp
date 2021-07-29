#include "ChessHistoryFinder.h"
#include "TextureManager.h"

namespace Chess {
    void ChessHistoryFinder::startProgramWindow() {
        window.create(sf::VideoMode(1280, 720), "Chess History Finder!");
        sf::Sprite bg(TextureManager::GetTexture("bg"));

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            window.draw(bg);
            window.display();
        }
    }
}
