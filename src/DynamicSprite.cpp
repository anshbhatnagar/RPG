#include "DynamicSprite.h"

void DynamicSprite::initialise(float speedVal, sf::Vector2f position, int sprSizeVal, sf::Texture& texture){
    speed = speedVal;
    Sprite::initialise(position, sprSizeVal, texture);
}

sf::FloatRect DynamicSprite::peekBounds(){
    sf::Transform moveMatrix;
    moveMatrix.translate(movement);
    return moveMatrix.transformRect(bounds);
}

void DynamicSprite::collisionMovement(sf::Vector2f moveVector){
    movement += moveVector;
}

void DynamicSprite::die(){
    dead = true;
}

void DynamicSprite::move(){
    sf::Transform moveMatrix;
    moveMatrix.translate(movement);
    bounds = moveMatrix.transformRect(bounds);
    sf::Sprite::move(movement);
}

void DynamicSprite::updateFrame(float dt){
    throw std::runtime_error("this Character base class function is not meant to be accessed directly!");
}
        
void DynamicSprite::calcMovement(float dt){
    throw std::runtime_error("this Character base class function is not meant to be accessed directly!");
}
