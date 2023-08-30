#pragma once
#include <SFML/Graphics.hpp>

class Sprite: public sf::Sprite{
    public:
        sf::FloatRect bounds;

        virtual void initialise(sf::Vector2f position, int sprSizeVal, sf::Texture& texture);

        sf::RectangleShape getBoundingShapeOutline();

    protected:
        int sprSize;
};