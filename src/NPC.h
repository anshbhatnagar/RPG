#pragma once
#include "Character.h"

class NPC: public Character{
    public:
        float elapsedMovementSecs = 0;
        bool talking = false;

        sf::FloatRect movableArea;

        void initialise(int healthVal, float speedVal, sf::FloatRect movableAreaVal, sf::Vector2f position, sf::Texture& texture);

        void defaultAnimate();

        void updateFrame(float dt);

        void calcMovement(float dt);

        std::string getDialogue();

        void stopDialogue();

    private:
        std::vector<std::string> dialogueTree{"Ah, traveller! Would you like a quest to complete?", "I'll take your silence as a yes!"};
        int dialogueTracker = -1;
};