#include "DrawImageInstruction.h"
#include "Arcadable.h"

DrawImageInstruction::DrawImageInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::DrawRect, await) {}
DrawImageInstruction::DrawImageInstruction() {}

void DrawImageInstruction::init(std::vector<unsigned short> ids) {
    this->xValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->yValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->imageValue = Arcadable::getInstance()->values.find(ids[2])->second;
}
std::vector<Executable>* DrawImageInstruction::getExecutables(bool async) {

    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {
        std::vector<short unsigned int>* data = static_cast<ImageValue*>(this->imageValue)->data->getValueArray();
        int x = static_cast<int>(this->xValue->getNumber());
        int y = static_cast<int>(this->yValue->getNumber());
        int w = static_cast<int>(static_cast<ImageValue*>(this->imageValue)->width->getNumber());
        int h = static_cast<int>(static_cast<ImageValue*>(this->imageValue)->height->getNumber());
        int k = static_cast<int>(static_cast<ImageValue*>(this->imageValue)->keyColor->getNumber());

        Arcadable::getInstance()->canvas->drawImage(x, y, w, h, k, data);
        return {};
    }, async, false, awaiting, NULL, NULL)};

    return &executables;

}
