#pragma once
#include "Enemy.h"
#include "NPC.h"

class Player: public Character{
    public:
        bool talking = false;
        
        void initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture);

        void updateFrame(float dt);

        void calcMovement(float dt);
        
        void attack(Character& monster);

        void attackNearbyEnemies(std::vector<Enemy*>& enemies);
    
    private:
        float hitms = 0;

        void attackAnimate();

        void deathAnimate();

        void defaultAnimate();
};