#ifndef _NEURONAL_NETWORK_H_
#define _NEURONAL_NETWORK_H_
#include <stdio.h>
#include <stdlib.h>
#define neral_network neuron*

struct neuron;
struct neuronal_connection;

enum neuron_type
{
    input,
    output,
    hidden
};

struct neuron
{
    double input;
    unsigned connectionCount;
    struct neural_connection *connections;
    unsigned isOutput;
};
struct neural_connection
{
	struct neuron start;
    struct neuron end;
    double w;
};
#endif