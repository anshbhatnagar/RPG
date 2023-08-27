#include <SFML/Graphics.hpp>
#include <iostream>

int sprSize = 48;
enum Direction {north, northeast, east, southeast, south, southwest, west, northwest};

class Character: public sf::Sprite{
    public:
        int health;
        float speed;

        void initialise(int healthVal, float speedVal, sf::Vector2f position, int sprSizeVal, std::string textureAddress){
            if(!texture.loadFromFile(textureAddress)){
                throw std::runtime_error("failed to load player sprite!");
            }
            sprSize = sprSizeVal;
            setTexture(texture);
            setTextureRect(sf::IntRect(0,0,sprSize,sprSize));
            health = healthVal;
            speed = speedVal;
            setScale(sf::Vector2f(2.f, 2.f));
            setPosition(position);
        }

    protected:
        int sprSize = 48;
        sf::Texture texture;
        float elapsedms = 0;
        int frame = 0;
        Direction dir = south;
        bool moving = false;
};

class Player: public Character{
    public:
        virtual void initialise(int healthVal, float speedVal, sf::Vector2f position){
            Character::initialise(healthVal, speedVal, position, 48, "sprites/characters/player.png");
        }

        void update(float dt){
            playerControl(dt);
            elapsedms += dt*1e3;
            frame = ((int)elapsedms)/100;
            if(frame==6){
                frame = 0;
                elapsedms = 0;
            }
            int state = 0;
            int flipped = 1;
            int movement = 0;

            if(moving){
                movement = 3;
            }

            switch(dir){
                case east:
                case southeast:
                case northeast:
                    state = 1;
                    break;
                case west:
                case southwest:
                case northwest:
                    state = 1;
                    flipped = -1;
                    frame++;
                    break;
                case north:
                    state = 2;
                    break;
                default:
                    state = 0;
                    break;
            }

            setTextureRect(sf::IntRect(frame*sprSize,(state+movement)*sprSize,flipped*sprSize,sprSize));
        }

    protected:
        void playerControl(float dt){
            sf::Transform translation;
            sf::Transform rotation;
            sf::Transform transform;
            sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
            sf::Vector2f movement = sf::Vector2f(0.f, 0.f);
            bool multikey = false;
            moving = false;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                moving = true;
                translation.translate(unit);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                    rotation.rotate(-45);
                    dir = northeast;
                    multikey = true;
                }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                    rotation.rotate(45);
                    dir = southeast;
                    multikey = true;
                }else{
                    dir = east;
                }
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                moving = true;
                translation.translate(unit);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                    rotation.rotate(-135);
                    dir = northwest;
                    multikey = true;
                }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                    rotation.rotate(135);
                    dir = southwest;
                    multikey = true;
                }else{
                    rotation.rotate(180);
                    dir = west;
                }
            }

            if(!multikey){
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                    moving = true;
                    translation.translate(unit);
                    rotation.rotate(-90);
                    dir = north;
                }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                    moving = true;
                    translation.translate(unit);
                    rotation.rotate(90);
                    dir = south;
                }
            }
            transform = rotation*translation;
            move(transform*movement);
        }
};

class NPC: public Character{
    public:
        virtual void initialise(int healthVal, float speedVal, sf::Vector2f position){
            Character::initialise(healthVal, speedVal, position, 32, "sprites/characters/slime.png");
        }

        void update(float dt){
            elapsedms += dt*1e3;
            npcControl(dt);
            frame = ((int)elapsedms)/100;
            if(frame==4){
                frame = 0;
                elapsedms = 0;
            }
            int state = 0;
            int flipped = 1;

            switch(dir){
                case west:
                case southwest:
                case northwest:
                    flipped = -1;
                    frame++;
                    break;
                default:
                    flipped = 1;
                    break;
            }

            setTextureRect(sf::IntRect(frame*sprSize,state*sprSize,flipped*sprSize,sprSize));
        }
    
    protected:
        void npcControl(float dt){
            int random = 0;
            srand((unsigned) time(NULL));

            random = rand() % 101;

            sf::Transform translation;
            sf::Transform rotation;
            sf::Transform transform;
            sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
            sf::Vector2f movement = sf::Vector2f(0.f, 0.f);

            if(moving){
                if(random < 20){
                    moving = false;
                }

                random = rand() % 101;
                translation.translate(unit);

                if(random < 13){
                    rotation.rotate(-45);
                    dir = northeast;
                }else if(random < 25){
                    rotation.rotate(45);
                    dir = southeast;
                }else if(random < 37){
                    dir = east;
                }else if(random < 50){
                    rotation.rotate(-135);
                    dir = northwest;
                }else if(random < 63){
                    rotation.rotate(135);
                    dir = southwest;
                }else if(random < 75){
                    rotation.rotate(180);
                    dir = west;
                }else if(random < 87){
                    rotation.rotate(-90);
                    dir = north;
                }else{
                    rotation.rotate(90);
                    dir = south;
                }

                transform = rotation*translation;
                move(transform*movement);

            }else if(random < 40){
                moving = true;
            }
        }
};

class Game{
    public:
        void run(){
            setup();
            mainLoop();
            shutDown();
        }

    private:
        int width = 800;
        int height = 600;
        sf::RenderWindow window{sf::VideoMode(width, height), "RPG!"};
        Player player;
        NPC slime;

        void setup(){
            player.initialise(100, 200, sf::Vector2f(0.f, 0.f));
            slime.initialise(50, 50, sf::Vector2f(5.f, 5.f));
        }

        void mainLoop(){
            sf::Clock clock;

            while(window.isOpen()){
                sf::Event event;
                while(window.pollEvent(event)){
                    if(event.type == sf::Event::Closed){
                        window.close();
                    }
                }

                sf::Time timeStep = clock.restart();
                float dt = timeStep.asMicroseconds();
                dt /= 1e6;

                player.update(dt);
                slime.update(dt);

                window.clear();
                window.draw(player);
                window.draw(slime);
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
    }catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}