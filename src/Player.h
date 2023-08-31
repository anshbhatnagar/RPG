#pragma once
#include "Enemy.h"
#include "NPC.h"

class Player: public Character{
    public:
        void initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture);

        void attackAnimate();

        void defaultAnimate();

        void updateFrame(float dt);

        void calcMovement(float dt);
        
        void attack(Character& monster);

        void attackNearbyEnemies(std::vector<Enemy>& enemies);
};