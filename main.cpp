#include <SFML/Graphics.hpp>
#include <iostream>

int sprSize = 48;
enum Direction {north, northeast, east, southeast, south, southwest, west, northwest, NULLDIR};

class Sprite: public sf::Sprite{
    public:
        sf::FloatRect bounds;

        virtual void initialise(sf::Vector2f position, int sprSizeVal, std::string textureAddress){
            if(!texture.loadFromFile(textureAddress)){
                throw std::runtime_error("failed to load player sprite!");
            }
            sprSize = sprSizeVal;
            setTexture(texture);
            setTextureRect(sf::IntRect(0,0,sprSize,sprSize));
            setScale(sf::Vector2f(2.f, 2.f));
            setPosition(position);
        }

        sf::RectangleShape getBoundingShape(){
            sf::RectangleShape boundingShape;
            boundingShape.setSize(sf::Vector2f(bounds.width, bounds.height));
            boundingShape.setPosition(sf::Vector2f(bounds.left, bounds.top));
            boundingShape.setFillColor(sf::Color::Transparent);
            boundingShape.setOutlineColor(sf::Color::Red);
            boundingShape.setOutlineThickness(1);
            return boundingShape;
        }

    protected:
        int sprSize;
        sf::Texture texture;
};

class DynamicSprite: public Sprite{
    public:
        float speed;

        virtual void initialise(float speedVal, sf::Vector2f position, bool movableVal, int sprSizeVal, std::string textureAddress){
            speed = speedVal;
            movable = movableVal;
            Sprite::initialise(position, sprSizeVal, textureAddress);
        }

        sf::FloatRect peekBounds(){
            sf::Transform moveMatrix;
            moveMatrix.translate(movement);
            return moveMatrix.transformRect(bounds);
        }

        void collisionMovement(sf::Vector2f moveVector){
            movement += moveVector;
        }

        virtual void move(){
            sf::Transform moveMatrix;
            moveMatrix.translate(movement);
            bounds = moveMatrix.transformRect(bounds);
            sf::Sprite::move(movement);
        }

        virtual void updateFrame(float dt){
            throw std::runtime_error("this Character base class function is not meant to be accessed directly!");
        }
        
        virtual void calcMovement(float dt){
            throw std::runtime_error("this Character base class function is not meant to be accessed directly!");
        }


    protected:
        float elapsedms = 0;
        int frame = 0;
        Direction dir = south;
        bool moving = false;
        bool movable = false;
        sf::Vector2f movement;
};


class Character: public DynamicSprite{
    public:
        int health;

        virtual void initialise(int healthVal, float speedVal, sf::Vector2f position, int sprSizeVal, std::string textureAddress){
            health = healthVal;
            DynamicSprite::initialise(speedVal, position, true, sprSizeVal, textureAddress);
        }

};

class Player: public Character{
    public:
        void initialise(int healthVal, float speedVal, sf::Vector2f position){
            int sprSize = 48;
            sf::Vector2f boundSize = sprSize*0.333f*sf::Vector2f(2.f, 2.7f);
            bounds = sf::FloatRect(position+boundSize, boundSize);
            Character::initialise(healthVal, speedVal, position, sprSize, "sprites/characters/player.png");
        }

        void updateFrame(float dt){
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

        void calcMovement(float dt){
            sf::Transform translation;
            sf::Transform rotation;
            sf::Transform transform;
            sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
            movement = sf::Vector2f(0.f, 0.f);
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
            movement = transform*movement;
        }
};

class NPC: public Character{
    public:
        void initialise(int healthVal, float speedVal, sf::Vector2f position){
            int sprSize = 32;
            sf::Vector2f boundSize = sprSize*0.4f*sf::Vector2f(1.7f, 1.9f);
            bounds = sf::FloatRect(position+boundSize, boundSize);
            Character::initialise(healthVal, speedVal, position, 32, "sprites/characters/slime.png");
        }

        void updateFrame(float dt){
            elapsedms += dt*1e3;
            
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

        void calcMovement(float dt){
            int random = 0;
            srand((unsigned) time(NULL));

            random = rand() % 101;

            sf::Transform translation;
            sf::Transform rotation;
            sf::Transform transform;
            sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
            movement = sf::Vector2f(0.f, 0.f);

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
                movement = transform*movement;

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
            slime.initialise(50, 50, sf::Vector2f(200.f, 200.f));
        }

        void resolveCollision(DynamicSprite* sprite1, DynamicSprite* sprite2){
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

        void updateDynamicSprites(DynamicSprite* sprite1, DynamicSprite* sprite2, float dt){
            sprite1->calcMovement(dt);
            sprite2->calcMovement(dt);

            resolveCollision(sprite1, sprite2);

            sprite1->updateFrame(dt);
            sprite2->updateFrame(dt);

            sprite1->move();
            sprite2->move();
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
                
                updateDynamicSprites(&player, &slime, dt);

                window.clear();
                window.draw(player);
                window.draw(slime);
                //window.draw(player.getBoundingShape());
                //window.draw(slime.getBoundingShape());
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