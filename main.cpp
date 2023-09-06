#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "src/RPG.h"

enum GameState {running, dialoguing};

class InterfaceBar: public sf::Sprite{
    public:
        void initialise(float value, sf::Texture& texture, sf::Vector2f position){
            playerValue = value;
            bar.create(50,10);
            bar.clear();
            base.setTexture(texture);
            base.setTextureRect(sf::IntRect(0,0,50,10));
            life.setTexture(texture);
            life.setTextureRect(sf::IntRect(0,10,50,10));
            border.setTexture(texture);
            border.setTextureRect(sf::IntRect(0,20,50,10));
            mask = sf::RectangleShape(sf::Vector2f(50.f,10.f));
            mask.setFillColor(sf::Color::Transparent);
            mask.setPosition(sf::Vector2f(life.getPosition().x + life.getGlobalBounds().width, 0.f));
            setScale(sf::Vector2f(2.f, 2.f));
            setPosition(position);
            update(playerValue);
        }

        void update(float value){
            life.setPosition(sf::Vector2f((value-playerValue)/2.f, 0));
            mask.setPosition(sf::Vector2f(life.getPosition().x + life.getGlobalBounds().width+1.f, 0.f));
            bar.clear(sf::Color::Transparent);
            bar.draw(base);
            bar.draw(life, baseAlpha);
            bar.draw(mask, sf::BlendNone);
            bar.draw(border);
            bar.display();
            setTexture(bar.getTexture());
        }
    
    private:
        float playerValue;
        sf::RenderTexture bar;
        sf::Sprite base;
        sf::Sprite life;
        sf::BlendMode baseAlpha = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero, sf::BlendMode::Add, sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add);
        sf::Sprite border;
        sf::RectangleShape mask;
};

class Game{
    public:
        void run(){
            setup();
            mainLoop();
            shutDown();
        }

    private:
        GameState gamestate = running;
        int screenWidth = 800;
        int screenHeight = 608;
        sf::RenderWindow window{sf::VideoMode(screenWidth, screenHeight), "RPG!"};
        Player player;
        InterfaceBar healthBar;
        InterfaceBar manaBar;
        NPC strawHat;
        NPC* talkingNPC;
        sf::Sprite dialogueBox;
        sf::Text speech;
        sf::Font dialogueFont;
        std::vector<Sprite> mapSprites;
        std::vector<Sprite> mapSolidSprites;
        std::vector<DynamicSprite*> dynSprites;
        std::vector<Sprite*> drawSprites;
        std::vector<sf::Drawable*> uiSprites;
        std::vector<Slime> slimes;
        std::vector<Skeleton> skeletons;
        std::vector<Projectile> projectiles;
        std::vector<Enemy*> enemies;
        std::vector<NPC*> NPCs;
        std::vector<sf::Texture> sheets;
        std::vector<Quest> questLog;

        void loadResources(){
            sf::Texture playerSheet;
            sf::Texture slimeSheet;
            sf::Texture grass;
            sf::Texture plainSheet;
            sf::Texture fenceSheet;
            sf::Texture strawHatSheet;
            sf::Texture dialogueBoxTexture;
            sf::Texture skeletonSheet;
            sf::Texture fireballSheet;
            sf::Texture healthBarSheet;
            sf::Texture manaBarSheet;

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
            if(!strawHatSheet.loadFromFile("sprites/characters/npc.png")){
                throw std::runtime_error("failed to load NPC sprite!");
            }
            if(!dialogueBoxTexture.loadFromFile("sprites/ui/dialogue.png")){
                throw std::runtime_error("failed to load dialogue box!");
            }
            if(!skeletonSheet.loadFromFile("sprites/characters/skeleton.png")){
                throw std::runtime_error("failed to load skeleton sprite!");
            }
            if(!fireballSheet.loadFromFile("sprites/projectiles/fireball.png")){
                throw std::runtime_error("failed to load fireball sprite!");
            }
            if(!healthBarSheet.loadFromFile("sprites/ui/healthbar.png")){
                throw std::runtime_error("failed to load health bar!");
            }
            if(!manaBarSheet.loadFromFile("sprites/ui/manabar.png")){
                throw std::runtime_error("failed to load mana bar!");
            }
            sheets.push_back(playerSheet);
            sheets.push_back(slimeSheet);
            sheets.push_back(grass);
            sheets.push_back(plainSheet);
            sheets.push_back(fenceSheet);
            sheets.push_back(strawHatSheet);
            sheets.push_back(dialogueBoxTexture);
            sheets.push_back(skeletonSheet);
            sheets.push_back(fireballSheet);
            sheets.push_back(healthBarSheet);
            sheets.push_back(manaBarSheet);

            if(!dialogueFont.loadFromFile("fonts/Ubuntu-Regular.ttf")){
                throw std::runtime_error("failed to load dialogue font!");
            }
        }

        void setup(){
            loadResources();
            window.setFramerateLimit(120);
            window.setVerticalSyncEnabled(true);

            MapHandler map = MapHandler("maps/map1.json", 16, 25, 19);

            map.loadMap(mapSprites, mapSolidSprites, sheets);

            dialogueBox.setTexture(sheets[6], true);
            dialogueBox.setScale(sf::Vector2f(2.f, 2.f));
            int dialoguePadding = 10;
            dialogueBox.setPosition(sf::Vector2f(0.5f*(screenWidth-dialogueBox.getGlobalBounds().width), screenHeight - dialogueBox.getGlobalBounds().height - dialoguePadding));

            speech.setFont(dialogueFont);
            speech.setCharacterSize(15);
            speech.setPosition(dialogueBox.getPosition() + 2.f*sf::Vector2f((float)dialoguePadding, (float)dialoguePadding));
            
            player.initialise(100, 200, sf::Vector2f(0.f, 0.f), sheets[0]);

            healthBar.initialise(player.health, sheets[9], sf::Vector2f(20.f, 20.f));
            uiSprites.push_back(&healthBar);
            manaBar.initialise(player.health, sheets[10], sf::Vector2f(20.f, 40.f));
            uiSprites.push_back(&manaBar);

            strawHat.initialise(100, 50, sf::FloatRect(sf::Vector2f(600.f, 0.f), sf::Vector2f(200.f, 200.f)), sf::Vector2f(700.f, 20.f), sheets[5]);

            NPCs.push_back(&strawHat);

            srand((unsigned) time(NULL));
            for(int i=0; i<4; i++){
                Slime slime;
                slime.initialise(sf::Vector2f(rand() % 500, rand() % 500), sheets[1]);
                slimes.push_back(slime);
            }

            for(int i=0; i<3; i++){
                Skeleton skeleton;
                skeleton.initialise(sf::Vector2f(rand() % 500, 400 + rand() % 200), sheets[7]);
                skeletons.push_back(skeleton);
            }
        }

        void resolveDynamicCollision(DynamicSprite* sprite1, DynamicSprite* sprite2){
            if(sprite1->solid && sprite2->solid){
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
        }

        void resolveStaticCollision(DynamicSprite* dynSprite, Sprite* staticSprite){
            if(dynSprite->solid){
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
        }

        void keepOnScreen(DynamicSprite* sprite){
            if(sprite->solid){
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
        }

        void prepareDynSpritesArray(){
            dynSprites.clear();
            dynSprites.push_back(&player);

            for(auto & enemy : enemies){
                dynSprites.push_back(enemy);
            }

            for(auto & nonplayer : NPCs){
                dynSprites.push_back(nonplayer);
            }

            for(auto & projectile : projectiles){
                dynSprites.push_back(&projectile);
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

        void updateEnemies(){
            enemies.clear();

            for(auto & enemy : slimes){
                enemies.push_back(&enemy);
            }

            for(auto & enemy : skeletons){
                enemy.checkPlayerNearby(player);

                if(!player.dead){
                    enemy.attackNearbyPlayer(player);
                }
                
                enemies.push_back(&enemy);
            }
        }

        void grimReaper(){
            int i = 0;
            for(auto & enemy : slimes){
                if(enemy.dead){
                    slimes.erase(slimes.begin()+i);
                    for(auto & quest : questLog){
                        quest.update(1);
                    }
                }else{
                    i++;
                }
            }

            i = 0;

            for(auto & enemy : skeletons){
                if(enemy.dead){
                    skeletons.erase(skeletons.begin()+i);
                }else{
                    i++;
                }
            }

            i = 0;

            for(auto & projectile : projectiles){
                if(projectile.dead){
                    projectiles.erase(projectiles.begin()+i);
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

        void startDialogue(){
            for(auto & npc : NPCs){
                if(player.inInteractionDistance(*npc)){
                    talkingNPC = npc;
                    gamestate = dialoguing;
                }
            }

            if(gamestate == dialoguing){
                talkingNPC->talking = true;
                player.talking = true;
                uiSprites.push_back(&dialogueBox);
                talkingNPC->nextDialogue(speech, questLog);
                uiSprites.push_back(&speech);
            }
        }

        void endDialogue(){
            talkingNPC->stopDialogue();
            player.talking = false;
            gamestate = running;
            uiSprites.clear();
            uiSprites.push_back(&healthBar);
            uiSprites.push_back(&manaBar);
        }

        void checkDialogue(){
            if(gamestate == dialoguing){
                if(!player.inInteractionDistance(*talkingNPC)){
                    endDialogue();
                }
            }
        }

        void checkProjectiles(){
            for(auto & proj : projectiles){
                for(auto & enemy : enemies){
                    if(proj.bounds.intersects(enemy->bounds)){
                        proj.dealDamage(enemy);
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
                        case sf::Event::KeyPressed:
                            if(event.key.code == sf::Keyboard::E){
                                if(gamestate == running){
                                    startDialogue();
                                }else if(gamestate == dialoguing){
                                    endDialogue();
                                }
                            }
                            break;
                        case sf::Event::KeyReleased:
                            if(event.key.code == sf::Keyboard::F){
                                if(gamestate == running){
                                    player.fireFireball(projectiles, window, sheets);
                                }
                            }
                            break;
                        case sf::Event::MouseButtonPressed:
                            if(event.mouseButton.button == sf::Mouse::Left){
                                if(gamestate == running){
                                    player.attackNearbyEnemies(enemies);
                                }else if(gamestate == dialoguing){
                                    talkingNPC->nextDialogue(speech, questLog);
                                }
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
                updateEnemies();
                updateDynamicSprites(dt);
                healthBar.update(player.health);
                manaBar.update(player.mana);
                layerSprites();

                checkDialogue();
                checkProjectiles();

                window.clear();

                for(auto & sprite : mapSprites){
                    window.draw(sprite);
                }

                for(auto & sprite : drawSprites){
                    window.draw(*sprite);
                }

                for(auto & sprite : uiSprites){
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