#pragma once
#define EvaluationValue_h
#include <Arcadable.h>

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
  pow,
  eq,
  ne,
  gt,
  lt,
  ge,
  le
}

class EvaluationValue: public NumberValueType {
	public:
    NumberValueTypePointer<NumberValueType> left;
    NumberValueTypePointer<NumberValueType> right;
    EvaluationOperator evaluationOperator;
    bool isStatic;

    double get();
    void set(double newValue);
    bool isTruthy();
    PixelValue();
    PixelValue(
        unsigned short ID,
        NumberValueTypePointer<NumberValueType> left,
        NumberValueTypePointer<NumberValueType> right,
        EvaluationOperator evaluationOperator,
        boolean isStatic
    );
  private:
    double _STATIC_RESULT;
};
