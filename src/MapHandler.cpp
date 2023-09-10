#include "MapHandler.h"
#include <iostream>

MapHandler::MapHandler(std::string mapFileName, int sprSizeVal, int widthVal, int heightVal){
    map = misc::readFile(mapFileName, "map");

    width = widthVal;
    height = heightVal;
    sprSize = sprSizeVal;
}

void MapHandler::loadMap(std::vector<Sprite>& mapSprites, std::vector<Sprite>& mapSolidSprites, std::vector<sf::Texture>& sheets){
    loadBase(mapSprites, sheets);
    loadSolidMap(mapSolidSprites, sheets);
}

void MapHandler::loadBase(std::vector<Sprite>& mapSprites, std::vector<sf::Texture>& sheets){
    int i = 0;
    int j = 0;
    int sprSheetRows = 6;

    for(auto & gid : map["layers"][0]["data"].get<std::vector<int>>()){
        Sprite block;
        int id = 0;
        int sheetID = 2;
        if(gid < map["tilesets"][1]["firstgid"].get<int>()){
            id = gid - map["tilesets"][0]["firstgid"].get<int>();
            sheetID = 2;
        }else if(gid < map["tilesets"][2]["firstgid"].get<int>()){
            id = gid - map["tilesets"][1]["firstgid"].get<int>();
            sheetID = 3;
        }
        
        block.initialise(sf::Vector2f(2*sprSize*i, 2*sprSize*j), sprSize, sheets[sheetID]);
        block.setTextureRect(sf::IntRect(sprSize*(id%sprSheetRows),sprSize*(id/sprSheetRows),sprSize,sprSize));

        block.setScale(sf::Vector2f(2, 2));
        mapSprites.push_back(block);

        i++;

        if(i == width){
            i=0;
            j++;
        }
    }
}

void MapHandler::loadSolidMap(std::vector<Sprite>& mapSolidSprites, std::vector<sf::Texture>& sheets){
    int sprSheetRows = 4;
    int sheetID = 4;
    int i = 0;
    int j = 0;
    
    for(auto & gid : map["layers"][1]["data"].get<std::vector<int>>()){
        if(!(gid==0)){
            Sprite block;
            int id = gid - map["tilesets"][2]["firstgid"].get<int>();
            sf::Vector2f blockPosition = sf::Vector2f(2*sprSize*i, 2*sprSize*j);
            sf::Vector2f boundSize = sf::Vector2f(2*sprSize, 2*sprSize);
            block.initialise(blockPosition, sprSize, sheets[sheetID]);
            block.setTextureRect(sf::IntRect(sprSize*(id%sprSheetRows),sprSize*(id/sprSheetRows),sprSize,sprSize));
            block.setScale(sf::Vector2f(2, 2));

            switch(id){
                case 14:
                    blockPosition += sf::Vector2f(0, 0.5f*sprSize);
                    boundSize = sf::Vector2f(2*sprSize, sprSize);
                    break;
                case 4:
                    blockPosition += sf::Vector2f(0.5f*sprSize, 0);
                    boundSize = sf::Vector2f(sprSize, 2*sprSize);
                    break;
                default:
                    blockPosition += sf::Vector2f(0.5f*sprSize, 0.5f*sprSize);
                    boundSize = sf::Vector2f(sprSize, sprSize);
                    break;
            }

            block.setBounds(blockPosition, boundSize);

            mapSolidSprites.push_back(block);
        }

        i++;

        if(i == width){
            i=0;
            j++;
        }
    }
}