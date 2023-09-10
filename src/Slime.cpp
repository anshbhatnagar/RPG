#include "Slime.h"

void Slime::initialise(sf::Vector2f position, sf::Texture& texture){
    int sprSize = 32;
    int healthVal = 50;
    int speedVal = 50;
    setBounds(position+sprSize*1.f*sf::Vector2f(0.7f, 1.f), sprSize*1.f*sf::Vector2f(0.7f, 0.5f));
    setScale(sf::Vector2f(2.f, 2.f));

    deathAction = Action(0.5f);
    deathAction.addAnimation(4, 5, frameLength, 5*frameLength);

    hitAction = Action(0.3f);
    hitAction.addAnimation(3, 3, frameLength, 3*frameLength);

    Character::initialise(healthVal, speedVal, position, sprSize, texture);
}

void Slime::defaultAnimate(){
    int resetFrame = 4;

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
    }
}

void Slime::updateFrame(float dt){
    updateActions(dt);

    int flipped = 1;
    int state;

    elapsedms += dt*1e3;
    frame = ((int)elapsedms)/frameLength;

    if(deathAction.state == RUNNING || dead){
        state = deathAction.row;
        frame = deathAction.getFrame();
    }else if(hitAction.state == RUNNING){
        state = hitAction.row;
        frame = hitAction.getFrame();
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