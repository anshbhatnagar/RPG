#pragma once
#include "Character.h"
#include "Quest.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <cmath>

using json = nlohmann::json;

class NPC: public Character{
    public:
        bool talking = false;
        sf::FloatRect movableArea;

        void initialise(std::string npcFileName, std::vector<sf::Texture>& sheets);

        void defaultAnimate();

        void updateFrame(float dt);

        void calcMovement(float dt);

        std::string nextDialogue(std::vector<Quest>& questLog, Character& player);

        void stopDialogue();

    private:
        std::string name;
        json npcData;
        json dialogueTree;
        Action wanderAction = Action(2.f);
};