#pragma once
#include "Character.h"

class NPC: public Character{
    float elapsedMovementSecs = 0;

    public:
        sf::FloatRect movableArea;

        void initialise(int healthVal, float speedVal, sf::FloatRect movableAreaVal, sf::Vector2f position, sf::Texture& texture);

        void defaultAnimate();

        void updateFrame(float dt);

        void calcMovement(float dt);
};