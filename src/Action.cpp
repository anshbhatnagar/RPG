#include "Action.h"

Action::Action(float waitTimeVal){
    wait = waitTimeVal;
    duration = waitTimeVal;
}

void Action::addAnimation(int rowVal, int resetFrameVal, int frameLengthms, int durationValms){
    row = rowVal;
    resetFrame = resetFrameVal;
    frameLength = frameLengthms;
    duration = ((float)durationValms)/1e3;
}

void Action::start(){
    if(state == PAUSED){
        elapsedSecs = 0;
        state = RUNNING;
    }
}

void Action::update(float dt){
    if(state == RUNNING){
        elapsedSecs += dt;

        if(elapsedSecs >= duration){
            complete();
        }
    }else if(state == COMPLETED || state == WAITING){
        elapsedSecs += dt;

        if(elapsedSecs >= wait){
            elapsedSecs = 0;
            state = PAUSED;
        }
    }
}

bool Action::confirmCompletion(){
    if(state == COMPLETED){
        state = WAITING;
        return true;
    }else{
        return false;
    }
}

int Action::getFrame(){
    float elapsedms = elapsedSecs * 1e3;

    frame = ((int)elapsedms)/frameLength;

    if(frame >= resetFrame){
        frame = resetFrame;
    }

    return frame;
}

void Action::complete(){
    state = COMPLETED;
}