#pragma once
#include "Sprite.h"

class DynamicSprite: public Sprite{
    public:
        float speed;
        bool solid = true;
        bool dead = false;

        virtual void initialise(float speedVal, sf::Vector2f position, int sprSizeVal, sf::Texture& texture);

        sf::FloatRect peekBounds();

        void collisionMovement(sf::Vector2f moveVector);

        void die();

        virtual void move();

        virtual void updateFrame(float dt);
        
        virtual void calcMovement(float dt);

    protected:
        int frameLength = 100;
        float elapsedms = 0;
        int frame = 0;
        Direction dir = south;
        bool startAnimation = false;
        bool moving = false;
        sf::Vector2f movement;
};