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
}

Value *v_add(Value *a, Value *b) {
  Value *result = value_init(a->data + b->data, ADD);
  result->prev[0] = a;
  result->prev[1] = b;
  return result;
}

Value *v_mult(Value *a, Value *b) {
  Value *result = value_init(a->data * b->data, MULT);
  result->prev[0] = a;
  result->prev[1] = b;
  return result;
}

Value *v_relu(Value *a) {
  Value *result = value_init(a->data > 0 ? a->data : 0, RELU);
  result->prev[0] = a;
  return result;
}
