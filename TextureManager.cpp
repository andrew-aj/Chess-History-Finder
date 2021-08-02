#include "TextureManager.h"

namespace Chess {
    //Stores the map that converts string to a texture
    unordered_map<string, sf::Texture> TextureManager::textures;

    //Loads a texture in given name
    void TextureManager::LoadTexture(string fileName) {
        //Default path
        string path = "images/";
        path += fileName + ".png";

        //Load the texture from file
        textures[fileName].loadFromFile(path);
    }

    //Gets the texture based on string name
    sf::Texture& TextureManager::GetTexture(string textureName) {
        //Load in the texture if not found
        if (textures.find(textureName) == textures.end())
            LoadTexture(textureName);

        //Return the texture
        return textures[textureName];
    }

    //Clears the texture
    void TextureManager::Clear() {
        textures.clear();
    }
}