#pragma once
#include "misc.h"

class Quest{
    public:
        bool complete = false;
        bool rewarded = false;
        std::string issuingNPC;
        std::string tag;
        QuestType type;
        MonsterType monster;
        int numberToKill;
        int numberKilled = 0;
        
        Quest();
        Quest(std::string questFileName, std::string npcName);

        void update(int killed);
    
    private:
        json questData;

};