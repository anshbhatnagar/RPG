#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include "Sprite.h"

using json = nlohmann::json;

class MapHandler{
    public:
        int sprSize;
        int width;
        int height;

        MapHandler(std::string mapFileName, int sprSizeVal, int widthVal, int heightVal);

        void loadMap(std::vector<Sprite>& mapSprites, std::vector<Sprite>& mapSolidSprites, std::vector<sf::Texture>& sheets);

        void loadBase(std::vector<Sprite>& mapSprites, std::vector<sf::Texture>& sheets);

        void loadSolidMap(std::vector<Sprite>& mapSolidSprites, std::vector<sf::Texture>& sheets);

    private:
        json map;
};