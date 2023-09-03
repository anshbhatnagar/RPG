#include "Enemy.h"

void Enemy::checkPlayerNearby(Character& player){
    if(!player.dead){
        sf::Vector2f disp = getRealPosition() - player.getRealPosition();

        float lockDist = 150;

        lockedOnPlayer = (disp.x*disp.x + disp.y*disp.y < lockDist*lockDist);

        attractionPoint = player.getRealPosition();
        
    }else{
        lockedOnPlayer = false;
    }
}

void Enemy::randomMovement(float dt){
    int random = 0;

    random = rand() % 101;

    elapsedMovementSecs += dt;

    sf::Transform translation;
    sf::Transform rotation;
    sf::Transform transform;
    sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
    sf::Vector2f oldMovement = movement;
    movement = sf::Vector2f(0.f, 0.f);

    if(moving && (elapsedMovementSecs > 2 || movement == oldMovement)){
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

    }else if(moving && elapsedMovementSecs < 2){
        movement = oldMovement;

    }else if(random < 40 && elapsedMovementSecs > 2){
        moving = true;
        elapsedMovementSecs = 0;
    }
}