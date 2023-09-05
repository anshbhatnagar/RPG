#pragma once
#include "DynamicSprite.h"
#include "Action.h"

class Character: public DynamicSprite{
    public:
        int health;

        virtual void initialise(int healthVal, float speedVal, sf::Vector2f position, int sprSizeVal, sf::Texture& texture);

        void wound(int damage);

        bool inInteractionDistance(Sprite& sprite);

    protected:
        Action hitAction = Action(0.f);
        Action deathAction = Action(0.f);
        Action attackAction = Action(1.f);

        void updateActions(float dt);
};
