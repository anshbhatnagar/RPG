#include <SFML/Graphics.hpp>
#include <iostream>
#include<cmath>

enum Direction {north, northeast, east, southeast, south, southwest, west, northwest, NULLDIR};
enum State {normal, attackState, hitState, dyingState, deadState};

class Sprite: public sf::Sprite{
    public:
        sf::FloatRect bounds;

        virtual void initialise(sf::Vector2f position, int sprSizeVal, sf::Texture& texture){
            sprSize = sprSizeVal;
            setTexture(texture);
            setTextureRect(sf::IntRect(0,0,sprSize,sprSize));
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
};

class DynamicSprite: public Sprite{
    public:
        float speed;
        bool dead = false;

        virtual void initialise(float speedVal, sf::Vector2f position, bool movableVal, int sprSizeVal, sf::Texture& texture){
            speed = speedVal;
            movable = movableVal;
            Sprite::initialise(position, sprSizeVal, texture);
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
        bool attacking = false;


        virtual void initialise(int healthVal, float speedVal, sf::Vector2f position, int sprSizeVal, sf::Texture& texture){
            health = healthVal;
            DynamicSprite::initialise(speedVal, position, true, sprSizeVal, texture);
        }

        void startDying(){
            dying = true;
        }

        void die(){
            currentState = deadState;
            dead = true;
        }

        void wound(int damage){
            health -= damage;

            if(!dying){
                if(health < 0){
                    startDying();
                }else{
                    beingHit = true;
                }
            }
        }

    protected:
        State currentState = normal;
        bool beingHit = false;
        bool dying = false;
};

class NPC: public Character{
    public:
        void initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture){
            int sprSize = 32;
            sf::Vector2f boundSize = sprSize*0.4f*sf::Vector2f(1.7f, 1.9f);
            bounds = sf::FloatRect(position+boundSize, boundSize);
            setScale(sf::Vector2f(2.f, 2.f));
            Character::initialise(healthVal, speedVal, position, 32, texture);
        }

        void hitAnimate(){
            int resetFrame = 3;

            if(currentState != hitState){
                frame = 0;
                elapsedms = 0;
                currentState = hitState;
            }

            if(frame >= resetFrame){
                frame = 0;
                elapsedms = 0;
                beingHit = false;
                currentState = normal;
            }
        }

        void deathAnimate(){
            int resetFrame = 5;

            if(currentState != dyingState){
                frame = 0;
                elapsedms = 0;
                currentState = dyingState;
            }

            if(frame >= resetFrame){
                frame = 0;
                elapsedms = 0;
                dying = false;
                die();
            }
        }

        void defaultAnimate(){
            int resetFrame = 4;

            if(frame >= resetFrame){
                frame = 0;
                elapsedms = 0;
            }
        }

        void updateFrame(float dt){
            int flipped = 1;
            int state;

            elapsedms += dt*1e3;
            frame = ((int)elapsedms)/100;

            if(dying){
                state = 4;
                deathAnimate();
            }else if(beingHit){
                state = 3;
                hitAnimate();
            }else{
                state = 0;
                defaultAnimate();
            }


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

class Player: public Character{
    public:
        void initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture){
            int sprSize = 48;
            sf::Vector2f boundSize = sprSize*0.333f*sf::Vector2f(2.f, 2.7f);
            bounds = sf::FloatRect(position+boundSize, boundSize);
            setScale(sf::Vector2f(2.f, 2.f));
            Character::initialise(healthVal, speedVal, position, sprSize, texture);
        }

        void attackAnimate(){
            int resetFrame = 4;

            if(currentState != attackState){
                frame = 0;
                elapsedms = 0;
                currentState = attackState;
            }

            if(frame >= resetFrame){
                frame = 0;
                elapsedms = 0;
                attacking = false;
                currentState = normal;
            }
        }

        void defaultAnimate(){
            int resetFrame = 6;

            if(frame >= resetFrame){
                frame = 0;
                elapsedms = 0;
            }
        }

        void updateFrame(float dt){
            int resetFrame = 6;
            int state = 0;
            int flipped = 1;

            elapsedms += dt*1e3;
            frame = ((int)elapsedms)/100;

            if(attacking){
                state = 6;
                attackAnimate();
            }else if(moving){
                state = 3;
                defaultAnimate();
            }else{
                state = 0;
                defaultAnimate();
            }

            switch(dir){
                case east:
                case southeast:
                case northeast:
                    state += 1;
                    break;
                case west:
                case southwest:
                case northwest:
                    state += 1;
                    flipped = -1;
                    frame++;
                    break;
                case north:
                    state += 2;
                    break;
                default:
                    state += 0;
                    break;
            }

            setTextureRect(sf::IntRect(frame*sprSize,state*sprSize,flipped*sprSize,sprSize));
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
        
        void attack(Character& monster){
            int weaponDamage = 15;
            monster.wound(weaponDamage);
        }

        void attackNearbyEnemies(std::vector<NPC>& enemies){
            attacking = true;

            for(auto & enemy : enemies){
                if(!enemy.dead){
                    float playerPosX = bounds.left+0.5f*bounds.width;
                    float playerPosY = bounds.top+0.5f*bounds.height;
                    float enemyPosX = enemy.bounds.left+0.5f*enemy.bounds.width;
                    float enemyPosY = enemy.bounds.top+0.5f*enemy.bounds.height;

                    float distX = abs(playerPosX - enemyPosX);
                    float distY = abs(playerPosY - enemyPosY);

                    if(distX < 1.2f*bounds.width and distY < 1.2f*bounds.height){
                        attack(enemy);
                    }
                }
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
        int screenWidth = 800;
        int screenHeight = 608;
        sf::RenderWindow window{sf::VideoMode(screenWidth, screenHeight), "RPG!"};
        Player player;
        std::vector<Sprite> mapSprites;
        std::vector<DynamicSprite*> dynSprites;
        std::vector<NPC> enemies;
        std::vector<sf::Texture> sheets;

        void loadTextures(){
            sf::Texture playerSheet;
            sf::Texture slimeSheet;
            sf::Texture grass;
            sf::Texture plainSheet;
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
            sheets.push_back(playerSheet);
            sheets.push_back(slimeSheet);
            sheets.push_back(grass);
            sheets.push_back(plainSheet);
        }

        void createMap(){
            int sprSize = 16;
            for(int i=0; i<screenWidth/sprSize/2; i++){
                for(int j=0; j<screenHeight/sprSize/2; j++){
                    Sprite grassBlock;
                    grassBlock.initialise(sf::Vector2f(2*sprSize*i, 2*sprSize*j), sprSize, sheets[2]);
                    grassBlock.setScale(sf::Vector2f(2, 2));
                    mapSprites.push_back(grassBlock);
                }
            }

            for(int i=0; i<screenWidth/sprSize/2; i++){
                Sprite upperEdgeBlock;
                Sprite lowerEdgeBlock;
                Sprite pathBlock;
                pathBlock.initialise(sf::Vector2f(2*sprSize*i, 2*sprSize*10), sprSize, sheets[3]);
                pathBlock.setTextureRect(sf::IntRect(sprSize*2,sprSize*1,sprSize,sprSize));
                pathBlock.setScale(sf::Vector2f(2, 2));
                mapSprites.push_back(pathBlock);
                upperEdgeBlock.initialise(sf::Vector2f(2*sprSize*i, 2*sprSize*9), sprSize, sheets[3]);
                upperEdgeBlock.setTextureRect(sf::IntRect(sprSize*2,sprSize*0,sprSize,sprSize));
                upperEdgeBlock.setScale(sf::Vector2f(2, 2));
                mapSprites.push_back(upperEdgeBlock);
                lowerEdgeBlock.initialise(sf::Vector2f(2*sprSize*i, 2*sprSize*11), sprSize, sheets[3]);
                lowerEdgeBlock.setTextureRect(sf::IntRect(sprSize*2,sprSize*2,sprSize,sprSize));
                lowerEdgeBlock.setScale(sf::Vector2f(2, 2));
                mapSprites.push_back(lowerEdgeBlock);
            }

        }

        void setup(){
            loadTextures();
            window.setFramerateLimit(120);
            window.setVerticalSyncEnabled(true);

            createMap();
            
            player.initialise(100, 200, sf::Vector2f(0.f, 0.f),sheets[0]);
            srand((unsigned) time(NULL));
            for(int i=0; i<4; i++){
                NPC slime;
                enemies.push_back(slime);
                enemies[i].initialise(50, 50, sf::Vector2f(rand() % 500, rand() % 500), sheets[1]);
            }
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
                        resolveCollision(sprite1, sprite2);
                    }
                    j++;
                }
                i++;
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

                window.clear();

                for(auto & sprite : mapSprites){
                    window.draw(sprite);
                }

                for(auto & sprite : dynSprites){
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