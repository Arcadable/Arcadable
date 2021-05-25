#include "EvaluationValue.h"
#include <cmath>
#include "../gameState.h"
EvaluationValue::EvaluationValue(
    unsigned short ID,
    EvaluationOperator evaluationOperator,
    bool isStatic,
    GameState *game
) : Value(ID, ValueType::evaluation) {
    this->evaluationOperator = evaluationOperator;
    this->isStatic = isStatic;
    this->_STATIC_RESULT = (double)(-999999999);
    this->isNumberType = true;
    this->game = game;
}
EvaluationValue::EvaluationValue() {
    this->_STATIC_RESULT = (double)(-999999999);
    this->isNumberType = true;
}
void EvaluationValue::init(std::vector<unsigned short> ids) {
    this->left = this->game->values.find(ids[0])->second;
    this->right = this->game->values.find(ids[1])->second;
};

double EvaluationValue::getNumber() {
    if (this->isStatic && this->_STATIC_RESULT != (double)(-999999999)) {
        return this->_STATIC_RESULT;
    }
    double left = this->left->getNumber();
    double right = this->right->getNumber();
    double result = -1;

    switch (this->evaluationOperator) {
        case EvaluationOperator::add:
            result = left + right;
            break;
        case EvaluationOperator::sub:
            result =  left - right;
            break;
        case EvaluationOperator::mul:
            result =  left * right;
            break;
        case EvaluationOperator::subdiv:
            result =  left / right;
            break;
        case EvaluationOperator::mod:
            result =  static_cast<int>(left) % static_cast<int>(right);
            break;
        case EvaluationOperator::b_and:
            result =  static_cast<int>(left) & static_cast<int>(right);
            break;
        case EvaluationOperator::b_or:
            result =  static_cast<int>(left) | static_cast<int>(right);
            break;
        case EvaluationOperator::b_xor:
            result =  static_cast<int>(left) ^ static_cast<int>(right);
            break;
        case EvaluationOperator::lsh:
            result =  static_cast<int>(left) << static_cast<int>(right);
            break;
        case EvaluationOperator::rsh:
            result =  static_cast<int>(left) >> static_cast<int>(right);
            break;
        case EvaluationOperator::poww:
            result =  std::pow(left, right);
            break;
        case EvaluationOperator::eq:
            result = (left == right) ? 1 : 0;
            break;
        case EvaluationOperator::ne:
            result = (left != right) ? 1 : 0;
            break;
        case EvaluationOperator::gt:
            result = (left > right) ? 1 : 0;
            break;
        case EvaluationOperator::lt:
            result = (left < right) ? 1 : 0;
            break;
        case EvaluationOperator::ge:
            result = (left >= right) ? 1 : 0;
            break;
        case EvaluationOperator::le:
            result = (left <= right) ? 1 : 0;
            break;
        default:
            result =  -1;
            break;
    }
    if (this->isStatic) {
        this->_STATIC_RESULT = result;
    }
    return result;
}
void EvaluationValue::setNumber(double newValue) {}
bool EvaluationValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* EvaluationValue::getValueArray() { return &this->emptyArray; };
void EvaluationValue::setValueArray(std::vector<unsigned short> newValue) { };