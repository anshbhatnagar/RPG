#include "Slime.h"

void Slime::initialise(sf::Vector2f position, sf::Texture& texture){
    int sprSize = 32;
    int healthVal = 50;
    int speedVal = 50;
    bounds = sf::FloatRect(position+sprSize*1.f*sf::Vector2f(0.7f, 1.f), sprSize*1.f*sf::Vector2f(0.7f, 0.5f));
    setScale(sf::Vector2f(2.f, 2.f));
    Character::initialise(healthVal, speedVal, position, sprSize, texture);
}


void Slime::hitAnimate(int& state){
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

void Slime::deathAnimate(){
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

void Slime::defaultAnimate(){
    int resetFrame = 4;

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
    }
}

void Slime::updateFrame(float dt){
    int flipped = 1;
    int state;

    elapsedms += dt*1e3;
    frame = ((int)elapsedms)/frameLength;

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

void Slime::calcMovement(float dt){
    randomMovement(dt);
}