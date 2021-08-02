#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

namespace Chess {
    class TextureManager {
        //Stores the map that converts string to a texture
        static unordered_map<string, sf::Texture> textures;

        //Loads a texture in given name
        static void LoadTexture(string textureName);
    public:
        //Gets the texture based on string name
        static sf::Texture& GetTexture(string textureName);

        //Clears the texture
        static void Clear();
    };
}
