#pragma once
#include "Enemy.h"
#include "NPC.h"
#include "Projectile.h"
#include <cmath>

class Player: public Character{
    public:
        int mana = 100;

        bool talking = false;

        bool inInventory = false;
        
        void initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture);

        void updateActions(float dt);

        void updateFrame(float dt);

        void calcMovement(float dt);
        
        void attack(Character& monster);

        void attackNearbyEnemies(std::vector<Enemy*>& enemies);

        void fireFireball(std::vector<Projectile>& projectiles, sf::RenderWindow& window, std::vector<sf::Texture>& sheets);
    
    private:
        Action magicCooldown = Action(1.5f);

        void defaultAnimate();
};