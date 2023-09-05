#pragma once
#include "Character.h"
#include "Action.h"

class Projectile: public DynamicSprite{
    public:
        Projectile(float angle, sf::Vector2f initialPos, sf::Texture& texture);
        
        void dealDamage(Character* victim);

        void defaultAnimate();

        void updateFrame(float dt);

        void calcMovement(float dt);

    private:
        sf::Transform rotation;
        Action formAction = Action(0.5f);
        Action durationAction = Action(4.f);
        float damage = 20.f;
};