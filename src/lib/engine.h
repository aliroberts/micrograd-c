#ifndef ENGINE_H
#define ENGINE_H

#include "darray.h"
#include "set.h"

// Declaration of the operation enumeration
typedef enum { NOOP, ADD, MULT, POW, EXP, RELU } Op;

// Declaration of the Value struct
typedef struct Value {
  double data;            // Value of the variable
  double grad;            // Gradient of the output with respect to this value
  struct Value *prev[2];  // Inputs to the operation that produced this value
  Op op;                  // Operation that produced this value
} Value;

// Function prototypes

// Initialize a new Value
Value *value_init(double data, Op op);

// Free a Value
void value_free(Value *value);

// Print a Value
void printv(Value *v);

// Perform the backward pass through the computational graph
void backward(Value *v);

// Add two Values
Value *v_add(Value *a, Value *b);

// Subtract b from a
Value *v_sub(Value *a, Value *b);

// Multiply two Values
Value *v_mult(Value *a, Value *b);

// Divide a by b
Value *v_div(Value *a, Value *b);

// Raise a to the power of b
Value *v_pow(Value *a, Value *b);

// Apply ReLU to a Value
Value *v_relu(Value *a);

// Apply the exponential function to a Value
Value *v_exp(Value *a);

#endif  // ENGINE_H
