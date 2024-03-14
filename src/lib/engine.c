#include "engine.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"
#include "set.h"

void backward_step(Value *v) {
  switch (v->op) {
    case ADD:
      v->prev[0]->grad += v->grad;
      v->prev[1]->grad += v->grad;
      break;
    case MULT:
      v->prev[0]->grad += v->grad * v->prev[1]->data;
      v->prev[1]->grad += v->grad * v->prev[0]->data;
      break;
    case POW:
      v->prev[0]->grad +=
          v->grad * v->data * pow(v->prev[0]->data, v->data - 1);
      v->prev[1]->grad += v->grad * log(v->prev[0]->data) * v->data;
      break;
    case EXP:
      v->prev[0]->grad += v->grad * exp(v->prev[0]->data);
      break;
    case RELU:
      v->prev[0]->grad += v->grad * (v->data > 0 ? 1 : 0);
      break;
    default:
      break;
  }
}

void printv(Value *v) {
  printf("Value{.data=%f, .grad=%f, .op=%i}\n", v->data, v->grad, v->op);
}

Value *value_init(double data, Op op) {
  Value *result = malloc(sizeof(Value));
  result->data = data;
  result->grad = 0.0;
  result->prev[0] = NULL;
  result->prev[1] = NULL;
  result->op = op;
  return result;
}

void value_free(Value *value) { free(value); }

void topo_sort(Value *v, DynamicArray *sorted, Set *visited) {
  if (set_contains(visited, v)) {
    return;
  }
  set_add(visited, v);
  if (v->prev[0] != NULL) {
    topo_sort(v->prev[0], sorted, visited);
  }
  if (v->prev[1] != NULL) {
    topo_sort(v->prev[1], sorted, visited);
  }
  darray_append(sorted, &v);
}

void backward(Value *v) {
  DynamicArray *sorted = darray_init(sizeof(Value **));
  Set *visited = set_init();
  topo_sort(v, sorted, visited);

  for (int i = sorted->size - 1; i >= 0; i--) {
    backward_step(*(Value **)darray_get(sorted, i));
  }
  darray_free(sorted);
  set_free(visited);
}

Value *v_add(Value *a, Value *b) {
  Value *result = value_init(a->data + b->data, ADD);
  result->prev[0] = a;
  result->prev[1] = b;
  return result;
}

Value *v_sub(Value *a, Value *b) {
  return v_add(a, v_mult(b, value_init(-1, NOOP)));
}

Value *v_mult(Value *a, Value *b) {
  Value *result = value_init(a->data * b->data, MULT);
  result->prev[0] = a;
  result->prev[1] = b;
  return result;
}

Value *v_pow(Value *a, Value *b) {
  Value *result = value_init(pow(a->data, b->data), POW);
  result->prev[0] = a;
  result->prev[1] = b;
  return result;
}

Value *v_div(Value *a, Value *b) {
  return v_mult(a, v_pow(b, value_init(-1, NOOP)));
}

Value *v_relu(Value *a) {
  Value *result = value_init(a->data > 0 ? a->data : 0, RELU);
  result->prev[0] = a;
  return result;
}

Value *v_exp(Value *a) {
  Value *result = value_init(exp(a->data), EXP);
  result->prev[0] = a;
  return result;
}
