#include "Character.h"

void Character::initialise(int healthVal, float speedVal, sf::Vector2f position, int sprSizeVal, sf::Texture& texture){
    health = healthVal;
    DynamicSprite::initialise(speedVal, position, sprSizeVal, texture);
}

void Character::die(){
    dead = true;
}

void Character::wound(int damage){
    health -= damage;
    startAnimation = true;

    if(currentState != dying){
        if(health < 0){
            currentState = dying;
        }else{
            currentState = wounding;
        }
    }
}

bool Character::inInteractionDistance(Sprite& sprite){
    sf::Vector2f dist = getRealPosition() - sprite.getRealPosition();

    return (abs(dist.x) < 1.3f*bounds.width && abs(dist.y) < 1.3f*bounds.height);
}