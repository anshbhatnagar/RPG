#pragma once

class NPC;

class Quest{
    public:
        bool complete = false;
        int numberToKill;
        int numberKilled = 0;
        NPC* issuingNPC;

        Quest();
        Quest(int numToKillVal, NPC* issuingNPCAddress);

        void update(int killed);

};