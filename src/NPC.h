#pragma once
#include "Character.h"
#include "Quest.h"

class NPC: public Character{
    public:
        bool talking = false;
        sf::FloatRect movableArea;
        Quest quest;
        bool questGiven = false;

        void initialise(int healthVal, float speedVal, sf::FloatRect movableAreaVal, sf::Vector2f position, sf::Texture& texture);

        void defaultAnimate();

        void updateFrame(float dt);

        void calcMovement(float dt);

        void nextDialogue(sf::Text& speech, std::vector<Quest>& questLog);

        void stopDialogue();

    private:
        Action wanderAction = Action(2.f);
        std::vector<std::string> dialogueTree{"Ah, traveller! Would you like a quest to complete?", "I'll take your silence as a yes!", "Here's a quest:\nKill two slimes and come back to me!"};
        int dialogueTracker = -1;
};