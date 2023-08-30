#include <SFML/Graphics.hpp>
#include "Sprite.h"

void Sprite::initialise(sf::Vector2f position, int sprSizeVal, sf::Texture& texture){
    sprSize = sprSizeVal;
    setTexture(texture);
    setTextureRect(sf::IntRect(0,0,sprSize,sprSize));
    setPosition(position);
}

sf::RectangleShape Sprite::getBoundingShapeOutline(){
    sf::RectangleShape boundingShape;
    boundingShape.setSize(sf::Vector2f(bounds.width, bounds.height));
    boundingShape.setPosition(sf::Vector2f(bounds.left, bounds.top));
    boundingShape.setFillColor(sf::Color::Transparent);
    boundingShape.setOutlineColor(sf::Color::Red);
    boundingShape.setOutlineThickness(1);
    return boundingShape;
}