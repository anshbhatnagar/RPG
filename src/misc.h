#pragma once
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

enum Direction {east, southeast, south, southwest, west, northwest, north, northeast, NULLDIR};

enum DialogueType {DEFAULT, CONDITION, QUEST, REWARD};
enum TestType {QUEST_COMPLETE, QUEST_GIVEN};
enum QuestType {KILL, FETCH};
enum MonsterType {SLIME, SKELETON};
enum Item {GOLD, WEAPON};

namespace misc{
    json readFile(std::string fileName, std::string errorType);

    QuestType map_QuestType(std::string stringVal);

    MonsterType map_MonsterType(std::string stringVal);

    DialogueType map_DialogueType(std::string stringVal);

    TestType map_TestType(std::string stringVal);

    Item map_Item(std::string stringVal);

}