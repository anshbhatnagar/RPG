#pragma once

class Character;

enum ActionState {PAUSED, RUNNING, COMPLETED, WAITING};

class Action{
    public:
        int row;

        ActionState state = PAUSED;

        Action(float waitTimeVal);

        void addAnimation(int rowVal, int resetFrameVal, int frameLength, int durationValms);

        void start();

        void update(float dt);

        bool confirmCompletion();

        int getFrame();
    
    private:
        int resetFrame;
        int frame;
        int frameLength;
        float duration = 0;
        float wait = 0;
        float elapsedSecs = 0;

        void complete();
};