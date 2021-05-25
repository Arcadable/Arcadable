#pragma once
#define InstructionSet_h
#include <vector>
class GameState;

class InstructionSet {
    public:
        unsigned short size;
        std::vector<unsigned short> instructions;
        GameState *game;
        
        unsigned short ID;
        bool async;
        std::vector<unsigned int> executables;
        std::vector<unsigned int> processedExecutables;

        std::vector<unsigned int>* getExecutables();
        InstructionSet();
        InstructionSet(
            unsigned short ID,
            unsigned short size,
            bool async,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);

    private: 
        void _processAwaiting(std::vector<unsigned int>* executables, std::vector<unsigned int>* out);


};
