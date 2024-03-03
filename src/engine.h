#ifndef ENGINE_H
#define ENGINE_H

#include "darray.h"
#include "set.h"

// Declaration of the operation enumeration
typedef enum { NOOP, ADD, MULT, RELU } Op;

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

// Print a Value
void printv(Value *v);

// Perform a backward step in the computational graph
void backward_step(Value *v);

// Perform the backward pass through the computational graph
void backward(Value *v);

// Add two Values
Value *v_add(Value *a, Value *b);

// Multiply two Values
Value *v_mult(Value *a, Value *b);

// Apply ReLU to a Value
Value *v_relu(Value *a);

#endif // ENGINE_H
