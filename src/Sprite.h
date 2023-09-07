#pragma once
#include <SFML/Graphics.hpp>
#include "misc.h"

class Sprite: public sf::Sprite{
    public:
        sf::FloatRect bounds;

        virtual void initialise(sf::Vector2f position, int sprSizeVal, sf::Texture& texture);

        sf::RectangleShape getBoundingShapeOutline();
        sf::Vector2f getRealPosition();

    protected:
        int sprSize;
};