#include "misc.h"

json misc::readFile(std::string fileName, std::string errorType){
    std::ifstream file;
    file.open(fileName);

    if(file.fail()){
        throw std::runtime_error("error loading " + errorType + " file!");
    }

    json data = json::parse(file);

    file.close();

    return data;
}

QuestType misc::map_QuestType(std::string stringVal){
    if(stringVal == "KILL"){
        return KILL;
    }else if(stringVal == "FETCH"){
        return FETCH;
    }else{
        throw std::runtime_error("invalid quest type!");
    }
}

MonsterType misc::map_MonsterType(std::string stringVal){
    if(stringVal == "SLIME"){
        return SLIME;
    }else if(stringVal == "SKELETON"){
        return SKELETON;
    }else{
        throw std::runtime_error("invalid monster type!");
    }
}

DialogueType misc::map_DialogueType(std::string stringVal){
    if(stringVal == "DEFAULT"){
        return DEFAULT;
    }else if(stringVal == "CONDITION"){
        return CONDITION;
    }else if(stringVal == "QUEST"){
        return QUEST;
    }else if(stringVal == "REWARD"){
        return REWARD;
    }else{
        throw std::runtime_error("invalid dialogue type!");
    }
}

TestType misc::map_TestType(std::string stringVal){
    if(stringVal == "QUEST_COMPLETE"){
        return QUEST_COMPLETE;
    }else if(stringVal == "QUEST_GIVEN"){
        return QUEST_GIVEN;
    }else{
        throw std::runtime_error("invalid worldstate test type!");
    }
}

Item misc::map_Item(std::string stringVal){
    if(stringVal == "GOLD"){
        return GOLD;
    }else if(stringVal == "WEAPON"){
        return WEAPON;
    }else{
        throw std::runtime_error("invalid item type!");
    }
}