#pragma once
#include "DynamicSprite.h"

enum State {normal, attacking, wounding, dying};

class Character: public DynamicSprite{
    public:
        int health;

        virtual void initialise(int healthVal, float speedVal, sf::Vector2f position, int sprSizeVal, sf::Texture& texture);

        void die();

        void wound(int damage);

        bool inInteractionDistance(Sprite& sprite);

    protected:
        int frameLength = 100;
        State currentState = normal;
};
