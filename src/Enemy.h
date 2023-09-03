#pragma once
#include "Character.h"

class Enemy: public Character{
    public:

        void checkPlayerNearby(Character& player);
    
    protected:
        float elapsedMovementSecs = 0;
        float waitingSecs = 0;
        bool lockedOnPlayer;
        sf::Vector2f attractionPoint;

        void randomMovement(float dt);
};
