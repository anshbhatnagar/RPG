#pragma once
#include "Enemy.h"

class Slime: public Enemy{
    public:
        void initialise(sf::Vector2f position, sf::Texture& texture);

        void updateFrame(float dt);

        void calcMovement(float dt);
    
    protected:
        void defaultAnimate();

};
