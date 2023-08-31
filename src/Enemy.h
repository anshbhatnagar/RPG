#pragma once
#include "Character.h"

class Enemy: public Character{
    public:
        void initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture);

        void hitAnimate(int& state);

        void deathAnimate();

        void defaultAnimate();

        void updateFrame(float dt);

        void calcMovement(float dt);
        
};
