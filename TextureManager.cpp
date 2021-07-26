//
// Created by hicke on 7/26/2021.
//

#include "TextureManager.h"

namespace Chess{
    void TextureManager::LoadTexture(string fileName) {
        string path = "./images/";
        path += fileName + ".png";
        textures[fileName].loadFromFile(path);
    }

    sf::Texture& TextureManager::GetTexture(string textureName) {
        LoadTexture(textureName);

        return textures[textureName];
    }

    void TextureManager::Clear() {
        textures.clear();
    }

}