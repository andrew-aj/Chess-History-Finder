//
// Created by hicke on 7/26/2021.
//

#ifndef CHESSHISTORYFINDER_TEXTUREMANAGER_H
#define CHESSHISTORYFINDER_TEXTUREMANAGER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>

using std::string;
using std::unordered_map;
using std::cout;
using std::endl;
using std::filesystem::directory_iterator;

namespace Chess{
    class TextureManager{
        static unordered_map<string, sf::Texture> textures;
        static void LoadTexture(string textureName);
        //static string imageDirectory;

    public:
        static sf::Texture& GetTexture(string TextureName);
        static void Clear(); //call me at the end of main
        static void LoadImages();

    };
}


#endif //CHESSHISTORYFINDER_TEXTUREMANAGER_H
