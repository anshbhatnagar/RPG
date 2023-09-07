#include "NPC.h"

const float PI = 3.14159;

void NPC::initialise(std::string npcFileName, std::vector<sf::Texture>& sheets){
    npcData = misc::readFile(npcFileName, "NPC");

    name = npcData["name"].get<std::string>();

    int sprSize = npcData["sprite_size"].get<int>();

    sf::Vector2f position = sf::Vector2f(npcData["coords"][0].get<float>(), npcData["coords"][1].get<float>());

    bounds = sf::FloatRect(position+sprSize*1.f*sf::Vector2f(0.66f, 1.4f), sprSize*1.f*sf::Vector2f(0.66f, 0.4f));

    setScale(sf::Vector2f(2.f, 2.f));

    json confinement = npcData["confinement"];
    dialogueTree = npcData["dialogue_tree"];

    movableArea = sf::FloatRect(sf::Vector2f(confinement["position"][0].get<float>(), confinement["position"][1].get<float>()), sf::Vector2f(confinement["size"][0].get<float>(), confinement["size"][1].get<float>()));

    int healthVal = npcData["health"].get<int>();
    int speedVal = npcData["speed"].get<int>();
    int sheetID = npcData["sheet_ID"].get<int>();

    Character::initialise(healthVal, speedVal, position, sprSize, sheets[sheetID]);
}

void NPC::defaultAnimate(){
    int resetFrame = 6;

    if(frame >= resetFrame){
        frame = 0;
        elapsedms = 0;
    }
}

void NPC::updateFrame(float dt){
    int resetFrame = 6;
    int state = 0;
    int flipped = 1;

    elapsedms += dt*1e3;
    frame = ((int)elapsedms)/100;

    if(moving){
        state = 3;
        defaultAnimate();
    }else{
        state = 0;
        defaultAnimate();
    }

    switch(dir){
        case east:
        case southeast:
        case northeast:
            state += 1;
            break;
        case west:
        case southwest:
        case northwest:
            state += 1;
            flipped = -1;
            frame++;
            break;
        case north:
            state += 2;
            break;
        default:
            state += 0;
            break;
    }

    setTextureRect(sf::IntRect(frame*sprSize,state*sprSize,flipped*sprSize,sprSize));
}

void NPC::calcMovement(float dt){
    wanderAction.update(dt);

    int random = 0;

    random = rand() % 101;

    sf::Transform translation;
    sf::Transform rotation;
    sf::Transform transform;
    sf::Vector2f unit = dt*speed*sf::Vector2f(1.f, 0.f);
    sf::Vector2f oldMovement = movement;
    movement = sf::Vector2f(0.f, 0.f);

    if(moving && (wanderAction.state != RUNNING || movement == oldMovement) && !talking){
        if(random < 50){
            moving = false;
        }

        translation.translate(unit);
        
        if(movableArea.intersects(peekBounds())){
            random = rand() % 101;

            if(random < 13){
                rotation.rotate(-45);
                dir = northeast;
            }else if(random < 25){
                rotation.rotate(45);
                dir = southeast;
            }else if(random < 37){
                dir = east;
            }else if(random < 50){
                rotation.rotate(-135);
                dir = northwest;
            }else if(random < 63){
                rotation.rotate(135);
                dir = southwest;
            }else if(random < 75){
                rotation.rotate(180);
                dir = west;
            }else if(random < 87){
                rotation.rotate(-90);
                dir = north;
            }else{
                rotation.rotate(90);
                dir = south;
            }
        }else{
            sf::Vector2f centre = sf::Vector2f(movableArea.left, movableArea.top) + 0.5f*sf::Vector2f(movableArea.width, movableArea.height);
            sf::Vector2f myCentre = sf::Vector2f(bounds.left, bounds.top) + 0.5f*sf::Vector2f(bounds.width, bounds.height);
            sf::Vector2f attraction = myCentre - centre;
            int attractionDir = ((int)(atan(attraction.y/attraction.x)/PI*180))/45;
            rotation.rotate(attractionDir*45);

            switch(attractionDir){
                case 0:
                case 8:
                    dir = east;
                    break;
                case 1:
                    dir = southeast;
                    break;
                case 2:
                    dir = south;
                    break;
                case 3:
                    dir = southwest;
                    break;
                case 4:
                    dir = west;
                    break;
                case 5:
                    dir = northwest;
                    break;
                case 6:
                    dir = north;
                    break;
                case 7:
                    dir = northeast;
                    break;
                default:
                    break;
            }
        }

        transform = rotation*translation;
        movement = transform*movement;

    }else if(moving && wanderAction.state == RUNNING && !talking){
        movement = oldMovement;
    }else if(random < 20 && wanderAction.state != RUNNING && !talking){
        moving = true;
        wanderAction.start();
    }else if(talking){
        moving = false;
    }
}

std::string NPC::nextDialogue(std::vector<Quest>& questLog, Character& player){
    
    if(dialogueTree.is_null()){
        return "";
    }else{
        for(int i = 0; i < dialogueTree.size(); i++){
            json dialogue = dialogueTree[i];
            DialogueType type = misc::map_DialogueType(dialogue["type"].get<std::string>());

            switch(type){
                case CONDITION:{
                    TestType test = misc::map_TestType(dialogue["test"].get<std::string>());
                    switch(test){
                        case QUEST_COMPLETE:
                            for(auto & quest : questLog){
                                if(quest.issuingNPC == name && quest.tag == dialogue["tag"].get<std::string>() && quest.complete){
                                    dialogueTree = dialogue["next"];
                                    return dialogue["line"].get<std::string>();
                                }else{
                                    continue;
                                }
                            }
                            continue;
                        case QUEST_GIVEN:
                            for(auto & quest : questLog){
                                if(quest.issuingNPC == name && quest.tag == dialogue["tag"].get<std::string>() && !quest.complete){
                                    dialogueTree = dialogue["next"];
                                    return dialogue["line"].get<std::string>();
                                }else{
                                    continue;
                                }
                            }
                            continue;
                        default:
                            throw std::runtime_error("invalid condition in dialogue for " + name + "!");
                            return "";
                    }
                }
                case QUEST:{
                    Quest newQuest = Quest("data/quests/" + dialogue["quest"].get<std::string>(), name);
                    questLog.push_back(newQuest);
                    dialogueTree = dialogue["next"];
                    return dialogue["line"].get<std::string>();
                }
                case REWARD:{
                    int value = dialogue["number"].get<int>();
                    Item itemType = misc::map_Item(dialogue["item"].get<std::string>());
                    for(auto & quest : questLog){
                        if(quest.issuingNPC == name && quest.tag == dialogue["tag"].get<std::string>() && quest.complete && !quest.rewarded){
                            player.give(itemType, value);
                            quest.rewarded = true;
                        }
                    }
                    dialogueTree = dialogue["next"];
                    return dialogue["line"].get<std::string>();
                }
                case DEFAULT:{
                    dialogueTree = dialogue["next"];
                    return dialogue["line"].get<std::string>();
                }
                default:{
                    throw std::runtime_error("invalid dialogue for " + name + "!");
                    return "";
                }
            }
        }
        return "";
    }
}

void NPC::stopDialogue(){
    talking = false;
    dialogueTree = npcData["dialogue_tree"];
}