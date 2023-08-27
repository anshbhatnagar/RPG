#include <SFML/Graphics.hpp>
#include <iostream>

int sprSize = 48;
enum Direction {north, northeast, east, southeast, south, southwest, west, northwest};

class Character: public sf::Sprite{
    public:
        int health;
        float speed;

        void update(float dt){
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

    protected:
        Direction dir = south;
        bool moving = false;
};

class Player: public Character{
    public:
        void initialise(int healthVal, float speedVal){
            if(!texture.loadFromFile("sprites/characters/player.png")){
                throw std::runtime_error("failed to load player sprite!");
            }
            setTexture(texture);
            setTextureRect(sf::IntRect(0,0,sprSize,sprSize));
            health = healthVal;
            speed = speedVal;
            setScale(sf::Vector2f(2.f, 2.f));
        }

        virtual void update(float dt){
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
            Character::update(dt);
        }
    
    private:
        sf::Texture texture;
        float elapsedms = 0;
        int frame = 0;
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
        sf::RenderWindow window{sf::VideoMode(width, height), "SFML Works!"};
        Player player;

        void setup(){
            player.initialise(100, 200);
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

                window.clear();
                window.draw(player);
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