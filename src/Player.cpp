#include "Player.h"


void Player::initialise(int healthVal, float speedVal, sf::Vector2f position, sf::Texture& texture){
    int sprSize = 48;
    bounds = sf::FloatRect(position+sprSize*1.f*sf::Vector2f(0.66f, 1.4f), sprSize*1.f*sf::Vector2f(0.66f, 0.4f));
    setScale(sf::Vector2f(2.f, 2.f));
    Character::initialise(healthVal, speedVal, position, sprSize, texture);
}

void Player::deathAnimate(){
    int resetFrame = 2;

    if(startAnimation){
        frame = 0;
        elapsedms = 0;
        startAnimation = false;
    }

    if(frame >= resetFrame){
        frame = resetFrame;
        elapsedms = resetFrame*frameLength;
        if(!dead){
            die();
        }
    }
}

void Player::attackAnimate(){
    int resetFrame = 4;

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

void Player::defaultAnimate(){
    int resetFrame = 6;

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
    }
}

void Player::updateFrame(float dt){
    int resetFrame = 6;
    int state = 0;
    int flipped = 1;

    elapsedms += dt*1e3;
    frame = ((int)elapsedms)/frameLength;

    if(currentState == dying){
        state = 9;
        deathAnimate();
    }else if(currentState == attacking){
        state = 6;
        attackAnimate();
    }else if(moving){
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

    if(currentState == dying){
        state = 9;
    }

    if(startAnimation){
        hitms = 0;
        startAnimation = false;
    }

    if(currentState == wounding){
        hitms += dt*1e3;
        if(((int)hitms)/frameLength % 2 == 0){
            setColor(sf::Color(255,0,0));
        }else{
            setColor(sf::Color(255,255,255));
        }
    }

    if(hitms > 4*frameLength){
        hitms = 0;
        currentState = normal;
        setColor(sf::Color(255,255,255));
    }

    if(dead){
        state = 9;
        frame = 2;

        if(flipped = -1){
            frame++;
        }
    }

    setTextureRect(sf::IntRect(frame*sprSize,state*sprSize,flipped*sprSize,sprSize));
}

void Player::calcMovement(float dt){
    sf::Transform translation;
    sf::Transform rotation;
    sf::Transform transform;
    sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
    movement = sf::Vector2f(0.f, 0.f);
    bool multikey = false;
    moving = false;

    if(!talking && !dead){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            moving = true;
            translation.translate(unit);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                rotation.rotate(-45);
                dir = northeast;
                multikey = true;
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                rotation.rotate(45);
                dir = southeast;
                multikey = true;
            }else{
                dir = east;
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            moving = true;
            translation.translate(unit);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                rotation.rotate(-135);
                dir = northwest;
                multikey = true;
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                rotation.rotate(135);
                dir = southwest;
                multikey = true;
            }else{
                rotation.rotate(180);
                dir = west;
            }
        }

        if(!multikey){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                moving = true;
                translation.translate(unit);
                rotation.rotate(-90);
                dir = north;
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                moving = true;
                translation.translate(unit);
                rotation.rotate(90);
                dir = south;
            }
        }
        transform = rotation*translation;
        movement = transform*movement;
    }
}

void Player::attack(Character& monster){
    int weaponDamage = 15;
    monster.wound(weaponDamage);
}

void Player::attackNearbyEnemies(std::vector<Enemy*>& enemies){
    if(!dead){
        currentState = attacking;
        startAnimation = true;

        for(auto & enemy : enemies){
            if(inInteractionDistance(*enemy)){
                attack(*enemy);
            }
        }
    }
}