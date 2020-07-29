#include "InstructionSet.h"
#include "Arcadable.h"

InstructionSet::InstructionSet (
    unsigned short ID,
    unsigned short size
) {
    this->size = size;
    this->ID = ID;
    this->instructions = std::vector<unsigned short>(this->size);

}

InstructionSet::InstructionSet() {}
void InstructionSet::init(std::vector<unsigned short> ids) {

    this->instructions = ids;
}
void InstructionSet::execute() {
    for ( auto instruction : this->instructions ) {
        Arcadable::getInstance()->instructions[instruction]->execute();
	}
}