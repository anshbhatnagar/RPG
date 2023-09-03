#include "Skeleton.h"

void Skeleton::initialise(sf::Vector2f position, sf::Texture& texture){
    int sprSize = 64;
    int healthVal = 100;
    int speedVal = 90;
    bounds = sf::FloatRect(position+sprSize*1.f*sf::Vector2f(0.8f, 1.47f), sprSize*1.f*sf::Vector2f(0.3f, 0.2f));
    setScale(sf::Vector2f(2.f, 2.f));
    Character::initialise(healthVal, speedVal, position, sprSize, texture);
}


void Skeleton::hitAnimate(int& state){
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

void Skeleton::attackAnimate(){
    int resetFrame = 5;

    if(startAnimation){
        frame = 0;
        elapsedms = 0;
        startAnimation = false;
    }

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
        currentState = normal;
    }
}

void Skeleton::deathAnimate(){
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

void Skeleton::defaultAnimate(int& state){
    int resetFrame = 6;

    if(moving){
        state+=1;
    }

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
    }
}

void Skeleton::updateFrame(float dt){
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
    }else if(currentState == attacking){
        state = 2;
        attackAnimate();
    }else{
        state = 0;
        defaultAnimate(state);
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

void Skeleton::trackMovement(float dt){
    sf::Transform translation;
    sf::Transform rotation;
    sf::Transform transform;
    sf::Vector2f movementUnit = dt*speed*sf::Vector2f(1.f, 0.f);
    
    sf::Vector2f attractionVec = attractionPoint - getRealPosition();

    sf::Transform rotateUnit;
    rotateUnit.rotate(45);

    sf::Vector2f unitVec = sf::Vector2f(1.f, 0.f);
    Direction bestDir = (Direction)0;
    float dotProduct = 0;

    movement = sf::Vector2f(0.f, 0.f);

    moving = true;

    for(int i = 0; i < 7; i++){
        float newDotProduct = unitVec.x*attractionVec.x + unitVec.y*attractionVec.y;

        if(newDotProduct > dotProduct){
            dotProduct = newDotProduct;
            bestDir = (Direction)i;
        }

        unitVec = rotateUnit*unitVec;
    }

    float avoidDist = 30;

    if(attractionVec.x*attractionVec.x+attractionVec.y*attractionVec.y < avoidDist*avoidDist){
        bestDir = (Direction)((int)bestDir + 2);
        if(bestDir == NULLDIR){
            bestDir = east;
        }
    }

    translation.translate(movementUnit);
    rotation.rotate(45*(int)bestDir);
    dir = bestDir;

    transform = rotation*translation;
    movement = transform*movement;
}

void Skeleton::calcMovement(float dt){
    waitingSecs += dt;
    
    if(lockedOnPlayer){
        trackMovement(dt);
    }else{
        randomMovement(dt);
    }
}

void Skeleton::attack(Character& player){
    int weaponDamage = 15;
    player.wound(weaponDamage);
}

void Skeleton::attackNearbyPlayer(Character& player){
    if(inInteractionDistance(player) && currentState != attacking && waitingSecs > 2){
        currentState = attacking;
        startAnimation = true;
        attack(player);
        waitingSecs = 0;
    }
}