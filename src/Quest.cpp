#include "Quest.h"

Quest::Quest(){
}

Quest::Quest(std::string questFileName, std::string npcName){
    questData = misc::readFile(questFileName, "quest");

    tag = questData["tag"].get<std::string>();
    type = misc::map_QuestType(questData["type"].get<std::string>());
    monster = misc::map_MonsterType(questData["object"].get<std::string>());

    numberToKill = questData["number"].get<int>();
    issuingNPC = npcName;
}

void Quest::update(int killed){
    numberKilled += killed;
    if(numberKilled >= numberToKill){
        complete = true;
    }
}