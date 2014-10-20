#ifndef _NEURONAL_NETWORK_H_
#define _NEURONAL_NETWORK_H_
#include <stdio.h>
#include <stdlib.h>
//#define netral_network struct neuron*

struct neuron;
struct neural_connection;

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
    struct neural_connection *connections;
    enum neuron_type type;
};

struct neural_connection
{
	struct neuron *start;
    struct neuron *end;
    double w;
};

struct neuron_output
{
    double *outputs;
    unsigned count;
};

void initWeights(struct neuron *network);
void displayWeights(struct neuron *network);

#endif