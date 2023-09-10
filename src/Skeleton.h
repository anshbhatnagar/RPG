#pragma once
#include "Enemy.h"

class Skeleton: public Enemy{
    public:
        void initialise(sf::Vector2f position, sf::Texture& texture);

        void updateFrame(float dt);

        void calcMovement(float dt);

        void attack(Character& player);

        void attackNearbyPlayer(Character& player);

        void generateDrop(std::vector<Sprite>& drops, std::vector<sf::Texture>& sheets);
    
    protected:
        void defaultAnimate(int& state);

        void trackMovement(float dt);
        
};