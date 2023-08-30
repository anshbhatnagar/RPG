#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "src/Player.h"

class Game{
    public:
        void run(){
            setup();
            mainLoop();
            shutDown();
        }

    private:
        int screenWidth = 800;
        int screenHeight = 608;
        sf::RenderWindow window{sf::VideoMode(screenWidth, screenHeight), "RPG!"};
        Player player;
        std::vector<Sprite> mapSprites;
        std::vector<Sprite> mapSolidSprites;
        std::vector<DynamicSprite*> dynSprites;
        std::vector<Sprite*> drawSprites;
        std::vector<NPC> enemies;
        std::vector<sf::Texture> sheets;

        void loadTextures(){
            sf::Texture playerSheet;
            sf::Texture slimeSheet;
            sf::Texture grass;
            sf::Texture plainSheet;
            sf::Texture fenceSheet;
            if(!playerSheet.loadFromFile("sprites/characters/player.png")){
                throw std::runtime_error("failed to load player sprite!");
            }
            if(!slimeSheet.loadFromFile("sprites/characters/slime.png")){
                throw std::runtime_error("failed to load slime sprite!");
            }
            if(!grass.loadFromFile("sprites/tilesets/grass.png")){
                throw std::runtime_error("failed to load grass texture!");
            }
            if(!plainSheet.loadFromFile("sprites/tilesets/plains.png")){
                throw std::runtime_error("failed to load plains textures!");
            }
            if(!fenceSheet.loadFromFile("sprites/tilesets/fences.png")){
                throw std::runtime_error("failed to load fences textures!");
            }
            sheets.push_back(playerSheet);
            sheets.push_back(slimeSheet);
            sheets.push_back(grass);
            sheets.push_back(plainSheet);
            sheets.push_back(fenceSheet);
        }

        std::vector<int> splitString(std::string givenStr, char delimiter){
            int i = 0;
            int j = 0;
            std::string tempStr;
            std::vector<int> elements;

            for(auto & stringChar : givenStr){
                if(stringChar == ','){
                    if(tempStr == ""){
                        throw std::runtime_error("error reading map file!");
                    }
                    elements.push_back(std::stoi(tempStr));
                    tempStr = "";
                    j=0;
                }else{
                    tempStr += stringChar;
                    i++;
                    j++;
                }
            }
            elements.push_back(std::stoi(tempStr));
            return elements;
        }

        void createMap(){
            int sprSize = 16;
            std::ifstream map;
            map.open("maps/map1.csv");
            if(map.fail()){
                throw std::runtime_error("error loading map file!");
            }
            std::string row;

            int j = 0;
            while(std::getline(map, row)){
                int i = 0;
                std::vector<int> ids = splitString(row, ',');
                for(auto & id : ids){
                    Sprite block;
                    if(id==0){
                        block.initialise(sf::Vector2f(2*sprSize*i, 2*sprSize*j), sprSize, sheets[2]);
                    }else{
                        id--;
                        block.initialise(sf::Vector2f(2*sprSize*i, 2*sprSize*j), sprSize, sheets[3]);
                        block.setTextureRect(sf::IntRect(sprSize*(id%6),sprSize*(id/6),sprSize,sprSize));
                    }
                    block.setScale(sf::Vector2f(2, 2));
                    mapSprites.push_back(block);
                    i++;
                }
                j++;
            }
            map.close();
        }

        void createSolidMap(){
            int sprSize = 16;
            int sprSheetRows = 4;
            std::ifstream map;
            map.open("maps/map1_solid.csv");
            if(map.fail()){
                throw std::runtime_error("error loading map solid layer file!");
            }
            std::string row;

            int j = 0;
            while(std::getline(map, row)){
                int i = 0;
                std::vector<int> ids = splitString(row, ',');
                for(auto & id : ids){
                    if(!(id==-1)){
                        Sprite block;
                        sf::Vector2f blockPosition = sf::Vector2f(2*sprSize*i, 2*sprSize*j);
                        sf::Vector2f boundSize = sf::Vector2f(2*sprSize, 2*sprSize);
                        block.initialise(blockPosition, sprSize, sheets[4]);
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

                        block.bounds=sf::FloatRect(blockPosition, boundSize);

                        mapSolidSprites.push_back(block);
                    }
                    i++;
                }
                j++;
            }
            map.close();
        }

        void setup(){
            loadTextures();
            window.setFramerateLimit(120);
            window.setVerticalSyncEnabled(true);

            createMap();
            createSolidMap();
            
            player.initialise(100, 200, sf::Vector2f(0.f, 0.f),sheets[0]);
            srand((unsigned) time(NULL));
            for(int i=0; i<4; i++){
                NPC slime;
                enemies.push_back(slime);
                enemies[i].initialise(50, 50, sf::Vector2f(rand() % 500, rand() % 500), sheets[1]);
            }
        }

        void resolveDynamicCollision(DynamicSprite* sprite1, DynamicSprite* sprite2){
            sf::FloatRect intersection;

            if(sprite1->peekBounds().intersects(sprite2->peekBounds(), intersection)){
                if(intersection.width < intersection.height){
                    if(sprite2->peekBounds().left < sprite1->peekBounds().left){
                        sprite1->collisionMovement(sf::Vector2f(intersection.width, 0.f));
                    }else{
                        sprite1->collisionMovement(-sf::Vector2f(intersection.width, 0.f));
                    }
                }else{
                    if(sprite2->peekBounds().top < sprite1->peekBounds().top){
                        sprite1->collisionMovement(sf::Vector2f(0.f, intersection.height));
                    }else{
                        sprite1->collisionMovement(-sf::Vector2f(0.f, intersection.height));
                    }
                }
            }
        }

        void resolveStaticCollision(DynamicSprite* dynSprite, Sprite* staticSprite){
            sf::FloatRect intersection;

            if(dynSprite->peekBounds().intersects(staticSprite->bounds, intersection)){

                if(intersection.width < intersection.height){
                    if(staticSprite->bounds.left < dynSprite->peekBounds().left){
                        dynSprite->collisionMovement(sf::Vector2f(intersection.width, 0.f));
                    }else{
                        dynSprite->collisionMovement(-sf::Vector2f(intersection.width, 0.f));
                    }
                }else{
                    if(staticSprite->bounds.top < dynSprite->peekBounds().top){
                        dynSprite->collisionMovement(sf::Vector2f(0.f, intersection.height));
                    }else{
                        dynSprite->collisionMovement(-sf::Vector2f(0.f, intersection.height));
                    }
                }
            }
        }

        void keepOnScreen(DynamicSprite* sprite){
            if(sprite->peekBounds().top < 0){
                sprite->collisionMovement(-sf::Vector2f(0.f, sprite->peekBounds().top));
            }else if(sprite->peekBounds().top + sprite->peekBounds().height > screenHeight){
                sprite->collisionMovement(-sf::Vector2f(0.f, sprite->peekBounds().top + sprite->peekBounds().height-screenHeight));
            }

            if(sprite->peekBounds().left < 0){
                sprite->collisionMovement(-sf::Vector2f(sprite->peekBounds().left, 0.f));
            }else if(sprite->peekBounds().left + sprite->peekBounds().width > screenWidth){
                sprite->collisionMovement(-sf::Vector2f(sprite->peekBounds().left + sprite->peekBounds().width-screenWidth, 0.f));
            }
        }

        void prepareDynSpritesArray(){
            dynSprites.clear();
            dynSprites.push_back(&player);

            for(auto & enemy : enemies){
                dynSprites.push_back(&enemy);
            }
        }

        void updateDynamicSprites(float dt){
            prepareDynSpritesArray();
            srand((unsigned) time(NULL));

            for(auto & sprite : dynSprites){
                sprite->calcMovement(dt);
            }

            int i = 0;
            for(auto & sprite1 : dynSprites){
                int j = 0;
                for(auto & sprite2 : dynSprites){
                    if(j>i){
                        resolveDynamicCollision(sprite1, sprite2);
                    }
                    j++;
                }
                i++;
            }

            for(auto & dynSprite : dynSprites){
                for(auto & staticSprite : mapSolidSprites){

                    resolveStaticCollision(dynSprite, &staticSprite);
                }
            }

            for(auto & sprite : dynSprites){
                keepOnScreen(sprite);
                sprite->updateFrame(dt);
                sprite->move();
            }
        }

        void grimReaper(){
            int i = 0;
            for(auto & enemy : enemies){
                if(enemy.dead){
                    enemies.erase(enemies.begin()+i);
                }else{
                    i++;
                }
            }
        }

        void layerSprites(){
            drawSprites.clear();
            for(auto & sprite : mapSolidSprites){
                drawSprites.push_back(&sprite);
            }

            for(auto & sprite : dynSprites){
                drawSprites.push_back(sprite);
            }

            int n = drawSprites.size();

            for(int i = 0; i < n-1; i++){
                for(int j = 0; j < n-1-i; j++){
                    Sprite* sprite1 = drawSprites[j];
                    Sprite* sprite2 = drawSprites[j+1];

                    float sprite1bottom = sprite1->getGlobalBounds().top + sprite1->getGlobalBounds().height;
                    float sprite2bottom = sprite2->getGlobalBounds().top + sprite2->getGlobalBounds().height;

                    if(sprite2bottom < sprite1bottom){

                        std::swap(drawSprites[j], drawSprites[j+1]);
                    }
                }
            }
        }

        void mainLoop(){
            sf::Clock clock;

            while(window.isOpen()){
                sf::Event event;

                while(window.pollEvent(event)){
                    if(event.type == sf::Event::Closed){
                        window.close();
                    }

                    switch(event.type){
                        case sf::Event::MouseButtonPressed:
                            if(event.mouseButton.button == sf::Mouse::Left){
                                player.attackNearbyEnemies(enemies);
                            }
                            break;

                        default:
                            break;
                    }
                }

                sf::Time timeStep = clock.restart();
                float dt = timeStep.asMicroseconds();
                dt /= 1e6;
                
                grimReaper();
                updateDynamicSprites(dt);
                layerSprites();

                window.clear();

                for(auto & sprite : mapSprites){
                    window.draw(sprite);
                }

                for(auto & sprite : drawSprites){
                    window.draw(*sprite);
                }

                window.display();
            }
        }

        void shutDown(){

        }

};

int main() {
    Game myGame;

    try{
        myGame.run();
    }catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}