#include "Enemy.h"


void Enemy::initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture){
    int sprSize = 32;
    bounds = sf::FloatRect(position+sprSize*1.f*sf::Vector2f(0.7f, 1.f), sprSize*1.f*sf::Vector2f(0.7f, 0.5f));
    setScale(sf::Vector2f(2.f, 2.f));
    Character::initialise(healthVal, speedVal, position, 32, texture);
}

void Enemy::hitAnimate(int& state){
    int resetFrame = 3;

    if(startAnimation){
        frame = 0;
        elapsedms = 0;
        startAnimation = false;
    }

    if(frame >= resetFrame){
        frame = 0;
        state = 0;
        elapsedms = 0;
        currentState = normal;
    }
}

void Enemy::deathAnimate(){
    int resetFrame = 5;

    if(startAnimation){
        frame = 0;
        elapsedms = 0;
        startAnimation = false;
    }

    if(frame >= resetFrame){
        frame = resetFrame;
        elapsedms = 0;
        die();
    }
}

void Enemy::defaultAnimate(){
    int resetFrame = 4;

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
    }
}

void Enemy::updateFrame(float dt){
    int flipped = 1;
    int state;

    elapsedms += dt*1e3;
    frame = ((int)elapsedms)/100;

    if(currentState == dying){
        state = 4;
        deathAnimate();
    }else if(currentState == wounding){
        state = 3;
        hitAnimate(state);
    }else{
        state = 0;
        defaultAnimate();
    }


    switch(dir){
        case west:
        case southwest:
        case northwest:
            flipped = -1;
            frame++;
            break;
        default:
            flipped = 1;
            break;
    }

    setTextureRect(sf::IntRect(frame*sprSize,state*sprSize,flipped*sprSize,sprSize));
    
}

void Enemy::calcMovement(float dt){
    int random = 0;

    random = rand() % 101;

    sf::Transform translation;
    sf::Transform rotation;
    sf::Transform transform;
    sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
    movement = sf::Vector2f(0.f, 0.f);

    if(moving){
        if(random < 20){
            moving = false;
        }

        random = rand() % 101;
        translation.translate(unit);

        if(random < 13){
            rotation.rotate(-45);
            dir = northeast;
        }else if(random < 25){
            rotation.rotate(45);
            dir = southeast;
        }else if(random < 37){
            dir = east;
        }else if(random < 50){
            rotation.rotate(-135);
            dir = northwest;
        }else if(random < 63){
            rotation.rotate(135);
            dir = southwest;
        }else if(random < 75){
            rotation.rotate(180);
            dir = west;
        }else if(random < 87){
            rotation.rotate(-90);
            dir = north;
        }else{
            rotation.rotate(90);
            dir = south;
        }

        transform = rotation*translation;
        movement = transform*movement;

    }else if(random < 40){
        moving = true;
    }
}