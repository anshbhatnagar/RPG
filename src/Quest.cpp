#include "Quest.h"

Quest::Quest(){
}

Quest::Quest(int numToKillVal, NPC* issuingNPCAddress){
    numberToKill = numToKillVal;
    issuingNPC = issuingNPCAddress;
}

void Quest::update(int killed){
    numberKilled += killed;
    if(numberKilled >= numberToKill){
        complete = true;
    }
}