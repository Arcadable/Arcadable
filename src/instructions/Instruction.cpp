#include "Instruction.h"
#include "Arcadable.h"

Instruction::Instruction (
    unsigned short ID,
    InstructionType type,
    bool await
) {
    this->type = type;
    this->ID = ID;
    this->await = await;
    this->executables = {};
    //this->awaiting = {};
    this->empty = {};

}
Instruction::Instruction () { }

std::vector<Executable>* Instruction::getExecutables(bool async) {
    for(auto &item : this->executables) {
        item.async = async;
    }
    return &this->executables;
}
void Instruction::init(std::vector<unsigned short> ids) {}