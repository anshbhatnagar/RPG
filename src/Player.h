#pragma once
#include "Enemy.h"
#include "NPC.h"

class Player: public Character{
    public:
        bool talking = false;
        
        void initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture);

        void attackAnimate();

        void defaultAnimate();

        void updateFrame(float dt);

        void calcMovement(float dt);
        
        bool inInteractionDistance(Sprite& sprite);
        
        void attack(Character& monster);

        void attackNearbyEnemies(std::vector<Enemy>& enemies);
};