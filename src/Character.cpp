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