#pragma once
#define EvaluationValue_h
#include <values/Value.h>
#include <vector>

enum EvaluationOperator {
  add,
  sub,
  mul,
  subdiv,
  mod,
  b_and,
  b_or,
  b_xor,
  lsh,
  rsh,
  poww,
  eq,
  ne,
  gt,
  lt,
  ge,
  le
};

class EvaluationValue: public Value {
	public:
    Value* left;
    Value* right;
    EvaluationOperator evaluationOperator;
    bool isStatic;

    double getNumber();
    void setNumber(double newValue);
    std::vector<unsigned short>* getValueArray();
    void setValueArray(std::vector<unsigned short> newValue);
    bool isTruthy();
    void init(std::vector<unsigned short> ids);
    
    EvaluationValue();
    EvaluationValue(
        unsigned short ID,
        EvaluationOperator evaluationOperator,
        bool isStatic
    );
  private:
    double _STATIC_RESULT;
};
