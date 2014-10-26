#ifndef _NEURONAL_NETWORK_H_
#define _NEURONAL_NETWORK_H_
#include <stdio.h>
#include <stdlib.h>

//struct neuron

enum neuron_type
{
    input,
    output,
    hidden
};

struct neuron
{
    double input;
    unsigned connectionsCount;
    double *w;
    enum neuron_type type;
    double delta;
};

struct neural_network
{
    struct neuron **neurons;
    unsigned inputCount;
    unsigned layerCount;
    unsigned neuronPerLayer;
};

struct neural_network *createNetwork(unsigned layerCount, unsigned inputCount,
        unsigned neuronCount);
double getOutput(struct neural_network *network);
void resetWeights(struct neural_network *network);
void freeNetwork(struct neural_network *network);
void resetNetwork(struct neural_network *network);
void learn(struct neural_network *network, double res);
double sigmoide(double s);
#endif
