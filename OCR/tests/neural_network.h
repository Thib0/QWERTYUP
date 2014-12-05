#ifndef _NEURONAL_NETWORK_H_
#define _NEURONAL_NETWORK_H_
#include <stdio.h>
#include <stdlib.h>

struct _neural_network
{
    double **output;
    double **delta;
    double ***weight;
    int nLayers;
    int *layersize;
    double beta;
    double alpha;
    double ***deltaweight_old;
};

typedef struct _neural_network neural_network;

neural_network *createNetwork(unsigned nLayers, int *layersize, double beta, double alpha);
void freeNetwork(neural_network *nn);

void runForward(neural_network *nn, double *in);
void runBackward(neural_network *nn, double *in, double *desired);
double getOutput(neural_network *nn, int i);
double evalError(neural_network *nn, double *desired);

int saveNetwork(neural_network *network);
neural_network *loadNetwork();

double sigmoide(double s);
double my_random();
#endif
