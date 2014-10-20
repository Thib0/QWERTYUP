#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neural_network.h"


void initWeights(struct neuron *network)
{
    if (network->type == output) {
        return;
    }
    for (int i = 0; i < network->connectionsCount; i++) {
        double r = rand();
        while (r > 1) {
            r /= 10;
        }
        network->connections[i].w = r;
        initWeights(network->connections[i].end);
    }
}


void displayWeights(struct neuron *network){
    for (int i = 0; i < network->connectionsCount; i++) {
        printf("%f\n",network->connections[i].w);
        displayWeights(network->connections[i].end);
    }
}

void addToOutput(struct neuron_output *out, double v)
{
    double *aux = malloc(sizeof(double) * (out->count + 1));
    aux = out->outputs;
    aux[out->count + 1] = v;
    out->outputs = aux;
    (out->count)++;
}
void resetInputs(struct neuron *network)
{
    if (network == NULL) {
        return;
    }
    for (int i = 0; i < network->connectionsCount; i++) {
        network->input = 0;
        resetInputs(network->connections[i].end);
    }
}








