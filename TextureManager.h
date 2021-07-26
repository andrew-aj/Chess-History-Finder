//
// Created by hicke on 7/26/2021.
//

#ifndef CHESSHISTORYFINDER_TEXTUREMANAGER_H
#define CHESSHISTORYFINDER_TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

namespace Chess{
    class TextureManager{
        static unordered_map<string, sf::Texture> textures;
        static void LoadTexture(string textureName);
        //static string imageDirectory;

    public:
        static sf::Texture& GetTexture(string TextureName);
        static void Clear(); //call me at the end of main
        static void LoadPieces();

    };
}


#endif //CHESSHISTORYFINDER_TEXTUREMANAGER_H
