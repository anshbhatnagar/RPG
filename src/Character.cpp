#include "Character.h"

void Character::initialise(int healthVal, float speedVal, sf::Vector2f position, int sprSizeVal, sf::Texture& texture){
    health = healthVal;
    DynamicSprite::initialise(speedVal, position, sprSizeVal, texture);
}

void Character::wound(int damage){
    health -= damage;

    if(deathAction.state == PAUSED){
        if(health < 0){
            deathAction.start();
        }else{
            hitAction.start();
        }
    }
}

bool Character::inInteractionDistance(Sprite& sprite){
    sf::Vector2f dist = getRealPosition() - sprite.getRealPosition();

    return (abs(dist.x) < 1.7f*bounds.width && abs(dist.y) < 1.7f*bounds.height);
}

void Character::updateActions(float dt){
    attackAction.update(dt);
    deathAction.update(dt);
    hitAction.update(dt);

    if(deathAction.confirmCompletion()){
        die();
    }
}