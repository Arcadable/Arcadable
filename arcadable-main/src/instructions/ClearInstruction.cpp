#include "ClearInstruction.h"
#include "../executable.h"

ClearInstruction::ClearInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display
) : Instruction(ID, InstructionType::Clear, await) {
    this->display = display;
}
ClearInstruction::ClearInstruction() {}

void ClearInstruction::init(std::vector<unsigned short> ids) {

    
}
std::vector<unsigned int>* ClearInstruction::action(bool async) {
    this->display->canvas.fillScreen(CRGB::Black);
    return &Executable::empty;


}
double ClearInstruction::getWaitAmount() {
    return 0;
}
