#include "Projectile.h"

Projectile::Projectile(float angle, sf::Vector2f initialPos, sf::Texture& texture){
    sprSize = 16;
    float speed = 300.f;
    rotation.rotate(angle);
    setRotation(angle);
    formAction.addAnimation(0, 5, 100, 5*100);
    formAction.start();
    durationAction.start();
    solid = false;
    setScale(2.f*sf::Vector2f(1.f, 1.f));
    setOrigin(sf::Vector2f(8,8));
    bounds = sf::FloatRect(initialPos-sf::Vector2f(8,8), sprSize*1.f*sf::Vector2f(1.f, 1.f));
    DynamicSprite::initialise(speed, initialPos, sprSize, texture);
}
        
void Projectile::dealDamage(Character* victim){
    victim->wound(damage);
    die();
}

void Projectile::defaultAnimate(){
    int resetFrame = 4;

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
    }
}

void Projectile::updateFrame(float dt){
    formAction.update(dt);
    durationAction.update(dt);

    if(durationAction.confirmCompletion()){
        die();
    }

    int state = 0;

    elapsedms += dt*1e3;
    frame = ((int)elapsedms)/frameLength;

    if(formAction.state == RUNNING){
        state = formAction.row;
        frame = formAction.getFrame();
    }else{
        state = 1;
        defaultAnimate();
    }

    setTextureRect(sf::IntRect(frame*sprSize,state*sprSize,sprSize,sprSize));

}

void Projectile::calcMovement(float dt){
    movement = rotation*(dt*speed*sf::Vector2f(1.f, 0.f));

}