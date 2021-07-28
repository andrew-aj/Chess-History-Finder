//
// Created by hicke on 7/26/2021.
//

#include "TextureManager.h"

namespace Chess{
    unordered_map<string, sf::Texture> TextureManager::textures;

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

    void TextureManager::LoadImages() {
        string path = "./images/";

        for(auto const& file : directory_iterator(path))
            cout << file.path() << endl;

    }

}