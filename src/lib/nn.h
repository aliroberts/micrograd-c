#include <stdlib.h>

#include "engine.h"

typedef struct {
  Value **values;
  int size;
} Vec;

Vec *vec_init(int size) {
  Vec *vec = malloc(sizeof(Vec));
  Value **params = calloc(size, sizeof(Value *));
  for (int i = 0; i < size; i++) {
    double uniform_random_val = 2 * ((double)rand() / (double)RAND_MAX) - 1;
    Value *value = value_init(uniform_random_val, NOOP);
    params[i] = value;
  }
  vec->values = params;
  vec->size = size;
  return vec;
}

void vec_free(Vec *vec) {
  for (int i = 0; i < vec->size; i++) {
    value_free(vec->values[i]);
  }
  free(vec->values);
  free(vec);
}

Vec *vec_exp(Vec *vec) {
  Vec *result = vec_init(vec->size);
  for (int i = 0; i < vec->size; i++) {
    result->values[i] = v_exp(vec->values[i]);
  }
  return result;
}

Value *vec_sum(Vec *vec) {
  Value *result = value_init(0, NOOP);
  for (int i = 0; i < vec->size; i++) {
    result = v_add(result, vec->values[i]);
  }
  return result;
}

Vec *vec_sub(Vec *vec, Value *val) {
  Vec *result = vec_init(vec->size);
  for (int i = 0; i < vec->size; i++) {
    result->values[i] = v_sub(vec->values[i], val);
  }
  return result;
}

Vec *vec_div(Vec *vec, Value *val) {
  Vec *result = vec_init(vec->size);
  for (int i = 0; i < vec->size; i++) {
    result->values[i] = v_div(vec->values[i], val);
  }
  return result;
}

Vec *vec_softmax(Vec *vec) {
  Value *max_val = vec->values[0];
  for (int i = 1; i < vec->size; i++) {
    if (vec->values[i]->data > max_val->data) {
      max_val = vec->values[i];
    }
  }

  Value *exp_const = value_init(max_val->data, NOOP);
  Vec *result = vec_exp(vec_sub(vec, exp_const));
  Value *sum = vec_sum(result);
  result = vec_div(result, sum);
  return result;
}

void print_vec(Vec *vec) {
  printf("{");
  for (int i = 0; i < vec->size; i++) {
    printf("%f, ", vec->values[i]->data);
  }
  printf("}\n");
}

typedef struct {
  Vec *params;
  int nin;
} Neuron;

Neuron *neuron_init(int nin) {
  Neuron *neuron = malloc(sizeof(Neuron));
  neuron->params = vec_init(nin + 1);
  neuron->params->values[nin]->data = 0.0;
  neuron->nin = nin;
  return neuron;
}

void neuron_free(Neuron *neuron) {
  vec_free(neuron->params);
  free(neuron);
}

Value *neuron_prod(Neuron *a, Vec *b) {
  Value *result = value_init(0, NOOP);
  for (int i = 0; i < a->nin; i++) {
    result = v_add(result, v_mult(a->params->values[i], b->values[i]));
  }
  result = v_add(result, a->params->values[a->nin]);
  return result;
}

typedef struct {
  Neuron **neurons;
  int nin;
  int nout;
  bool nonlin;
} Layer;

Layer *layer_init(int nin, int nout, bool nonlin) {
  Layer *layer = malloc(sizeof(Layer));
  Neuron **neurons = calloc(nout, sizeof(Neuron *));
  for (int i = 0; i < nout; i++) {
    neurons[i] = neuron_init(nin);
  }
  layer->neurons = neurons;
  layer->nin = nin;
  layer->nout = nout;
  layer->nonlin = nonlin;
  return layer;
}

void layer_free(Layer *layer) {
  for (int i = 0; i < layer->nout; i++) {
    neuron_free(layer->neurons[i]);
  }
  free(layer->neurons);
  free(layer);
}

Vec *layer_prod(Layer *layer, Vec *input) {
  Vec *result = vec_init(layer->nout);
  for (int i = 0; i < layer->nout; i++) {
    result->values[i] = neuron_prod(layer->neurons[i], input);
  }
  if (layer->nonlin) {
    for (int i = 0; i < layer->nout; i++) {
      // Note that each value we're overwriting here will remain in the graph
      // and so there won't be a memory leak.
      result->values[i] = v_relu(result->values[i]);
    }
  }
  return result;
}

typedef struct {
  Layer **layers;
  int nin;
  int *nouts;
  int nlayers;
} Network;

Network *network_init(int nin, int *nouts, int nlayers) {
  Network *network = malloc(sizeof(Network));
  Layer **layers = calloc(nlayers, sizeof(Layer *));
  for (int i = 0; i < nlayers; i++) {
    int layer_inputs = i == 0 ? nin : nouts[i - 1];
    bool nonlin = i < nlayers - 1;
    layers[i] = layer_init(layer_inputs, nouts[i], nonlin);
    nin = nouts[i];
  }
  network->layers = layers;
  network->nin = nin;
  network->nouts = nouts;
  network->nlayers = nlayers;
  return network;
}

Vec *network_call(Network *network, Vec *input) {
  Vec *result = input;
  for (int i = 0; i < network->nlayers; i++) {
    result = layer_prod(network->layers[i], result);
  }
  return result;
}

void network_free(Network *network) {
  for (int i = 0; i < network->nlayers; i++) {
    layer_free(network->layers[i]);
  }
  free(network->layers);
  free(network->nouts);
  free(network);
}

// Print the neuron as an array
void print_neuron(Neuron *neuron) {
  printf("{");
  for (int i = 0; i <= neuron->nin; i++) {
    printf("%f, ", neuron->params->values[i]->data);
  }
  printf("}\n");
}

void print_network(Network *network) {
  for (int i = 0; i < network->nlayers; i++) {
    printf("Layer %i\n", i);
    Layer *layer = network->layers[i];
    for (int j = 0; j < layer->nout; j++) {
      printf("Neuron %i\n", j);
      print_neuron(layer->neurons[j]);
    }
  }
}
