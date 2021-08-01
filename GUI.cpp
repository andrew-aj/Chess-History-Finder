#include "GUI.h"
#include "TextureManager.h"

namespace Chess {
    void GUI::startProgramWindow() {
        window.create(sf::VideoMode(1280, 720), "Chess History Finder!");
        sf::Sprite bg(TextureManager::GetTexture("bg"));
        window.setFramerateLimit(60);

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

    void GUI::readFileBinary(const string &fileName) {
        std::ifstream file(fileName, std::ios::binary);
        uint64_t totalSuccessfulHashes = 0;

        file.read((char*)ZobristHash::randNums.data(), 781*8);
        file.read((char *)&totalSuccessfulHashes, 8);

        sort.reserve(totalSuccessfulHashes);

        int loc = file.tellg();

        hash tempHash;
        NextMove tempMove;

        for(uint64_t i = 0; i < totalSuccessfulHashes; i++){
            //std::cout << i << std::endl;
            file.read((char*)&tempHash, 8);
            file.read((char*)&tempMove, 2);
            tree.insertHash(tempHash, tempMove);
        }

        file.seekg(loc);

        for(uint64_t i = 0; i < totalSuccessfulHashes; i++){
            file.read((char*)&sort[i].Data, 8);
            file.read((char*)&sort[i].bestMove, 2);
        }
    }
}
