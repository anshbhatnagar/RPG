#include "NPC.h"
#include <cmath>

const float PI = 3.14159;

void NPC::initialise(int healthVal, float speedVal, sf::FloatRect movableAreaVal, sf::Vector2f position, sf::Texture& texture){
    int sprSize = 48;
    bounds = sf::FloatRect(position+sprSize*1.f*sf::Vector2f(0.66f, 1.4f), sprSize*1.f*sf::Vector2f(0.66f, 0.4f));
    setScale(sf::Vector2f(2.f, 2.f));
    movableArea = movableAreaVal;
    Character::initialise(healthVal, speedVal, position, sprSize, texture);
}

void NPC::defaultAnimate(){
    int resetFrame = 6;

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
    }
}

void NPC::updateFrame(float dt){
    int resetFrame = 6;
    int state = 0;
    int flipped = 1;

    elapsedms += dt*1e3;
    frame = ((int)elapsedms)/100;

    if(moving){
        state = 3;
        defaultAnimate();
    }else{
        state = 0;
        defaultAnimate();
    }

    switch(dir){
        case east:
        case southeast:
        case northeast:
            state += 1;
            break;
        case west:
        case southwest:
        case northwest:
            state += 1;
            flipped = -1;
            frame++;
            break;
        case north:
            state += 2;
            break;
        default:
            state += 0;
            break;
    }

    setTextureRect(sf::IntRect(frame*sprSize,state*sprSize,flipped*sprSize,sprSize));
}

void NPC::calcMovement(float dt){
    int random = 0;

    random = rand() % 101;

    elapsedMovementSecs += dt;

    sf::Transform translation;
    sf::Transform rotation;
    sf::Transform transform;
    sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
    sf::Vector2f oldMovement = movement;
    movement = sf::Vector2f(0.f, 0.f);

    if(moving && (elapsedMovementSecs > 2 || movement == oldMovement) && !talking){
        if(random < 50){
            moving = false;
            elapsedMovementSecs = 0;
        }

        translation.translate(unit);
        
        if(movableArea.intersects(peekBounds())){
            random = rand() % 101;

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
        }else{
            sf::Vector2f centre = sf::Vector2f(movableArea.left, movableArea.top) + 0.5f*sf::Vector2f(movableArea.width, movableArea.height);
            sf::Vector2f myCentre = sf::Vector2f(bounds.left, bounds.top) + 0.5f*sf::Vector2f(bounds.width, bounds.height);
            sf::Vector2f attraction = myCentre - centre;
            int attractionDir = ((int)(atan(attraction.y/attraction.x)/PI*180))/45;
            rotation.rotate(attractionDir*45);

            switch(attractionDir){
                case 0:
                case 8:
                    dir = east;
                    break;
                case 1:
                    dir = southeast;
                    break;
                case 2:
                    dir = south;
                    break;
                case 3:
                    dir = southwest;
                    break;
                case 4:
                    dir = west;
                    break;
                case 5:
                    dir = northwest;
                    break;
                case 6:
                    dir = north;
                    break;
                case 7:
                    dir = northeast;
                    break;
                default:
                    break;
            }
        }

        transform = rotation*translation;
        movement = transform*movement;

    }else if(moving && elapsedMovementSecs < 2 && !talking){
        movement = oldMovement;

    }else if(random < 20 && elapsedMovementSecs > 2 && !talking){
        moving = true;
        elapsedMovementSecs = 0;
    }else if(talking){
        moving = false;
    }
}

std::string NPC::getDialogue(){
    if(dialogueTracker < (int)dialogueTree.size()-1){
        dialogueTracker += 1;
    }
    return dialogueTree[dialogueTracker];
}

void NPC::stopDialogue(){
    talking = false;
    dialogueTracker = -1;
}