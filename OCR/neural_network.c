#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neural_network.h"
#include <math.h>
#include <unistd.h>
#include <string.h>
#define alpha 0.35

double my_random()
{
    double r = rand();
    int sign = rand()%2;
    sign = sign == 1 ? 1 : -1;
    while (r > 0.5) {
        r /= 10;
    }
    r *= rand();
    while(r > 1)
        r /= 10;
    if (r > 0.43 && r < 0.63) {
        int s = rand();
        while (s > 1) {
            s /= 10;
        }
        int sign = r < 0.5 ? -1 : 1;
        r += s*sign;
    }
    return sign*r;
}

struct neural_network *createNetwork(unsigned layerCount, unsigned inputCount,
        unsigned neuronCount, unsigned outputCount)
{
    // allocs
    printf("lol");
    struct neuron **layers = malloc(sizeof(struct neuron*) * layerCount);
    printf("creating");
    
    layers[0] = malloc(sizeof(struct neuron) * inputCount);
    for (unsigned i = 1; i < layerCount - 1; i++) {
        layers[i] = malloc(sizeof(struct neuron) * neuronCount);
    }
    printf("creating");
    layers[layerCount - 1] = malloc(sizeof(struct neuron)*outputCount);
    printf("creating");
    // inits
    // input layer
    for (unsigned i = 0; i < inputCount; i++) {
        layers[0][i].input = 0;
        layers[0][i].type = input;
        layers[0][i].connectionsCount = neuronCount;
        layers[0][i].delta = 0;
        layers[0][i].w = malloc(sizeof(double) * neuronCount);
        for (unsigned k = 0; k < neuronCount; k++) {
            layers[0][i].w[k] = my_random();
        }
    }
    // hidden layers
    for (unsigned i = 1; i < layerCount - 2; i++) {
        for (unsigned j = 0; j < neuronCount; j++) {
            layers[i][j].input = 0;
            layers[i][j].type = hidden;
            layers[i][j].connectionsCount = neuronCount;
            layers[i][j].delta = 0;
            layers[i][j].w = malloc(sizeof(double) * neuronCount);
            for (unsigned k = 0; k < neuronCount; k++) {
                layers[i][j].w[k] = my_random();
            }
        }
    }
    printf("last hidden");
    // last hidden layer
    for (unsigned i = 0; i < neuronCount; i++) {
        layers[layerCount - 2][i].input = 0;
        layers[layerCount - 2][i].type = hidden;
        layers[layerCount - 2][i].connectionsCount = 1;
        layers[layerCount - 2][i].delta = 0;
        layers[layerCount - 2][i].w = malloc(sizeof(double)*outputCount);
        for (int j = 0; j < outputCount; i++) {
            layers[layerCount - 2][i].w[i] = my_random();
        }
        
    }

    // output layer
    layers[layerCount - 1][1].input = 0;
    layers[layerCount - 1][1].type = output;
    layers[layerCount - 1][1].connectionsCount = 0;
    layers[layerCount - 2][1].delta = 0;
    layers[layerCount - 1][1].w = NULL;

    struct neural_network *new_network = malloc(sizeof(struct neural_network));
    new_network->neurons = layers;
    new_network->inputCount = inputCount;
    new_network->layerCount = layerCount;
    new_network->neuronPerLayer = neuronCount;
    printf("created");
    return new_network;
}

double *getOutput(struct neural_network *network)
{
    struct neuron **neurons = network->neurons;
    for (unsigned i = 0; i < network->inputCount; i++) {
        for (unsigned j = 0; j < network->neuronPerLayer; j++) {
            neurons[1][j].input += neurons[0][i].w[j] * neurons[0][i].input;
        }
    }
    // hidden layers
    for (unsigned k = 1; k < network->layerCount - 2; k++) {
        for (unsigned i = 0; i < network->neuronPerLayer; i++) {
            for (unsigned j = 0; j < network->neuronPerLayer; j++) {
                neurons[k+1][j].input += neurons[k][i].w[j] *
                    sigmoide(neurons[k][i].input);
            }
        }
    }
    // last hidden layer
    printf("last hidden");
    unsigned n = network->layerCount;
    for (unsigned i = 0; i < network->neuronPerLayer; i++) {
        for (unsigned j = 0; j < network->outputCount; j++) {
            neurons[n - 1][j].input += neurons[n - 2][i].w[j] *
            sigmoide(neurons[n - 2][j].input);
        }
        
    }
    
    double *output = malloc(sizeof(double)*network->outputCount);
    for (int i = 0; i < network->outputCount; i++) {
        output[i] = sigmoide(network->neurons[n - 1][i].input);
    }
    
    return output;
}

void freeNetwork(struct neural_network *network)
{
    for(unsigned i = 0; i < network->inputCount; i++)
    {
        free(network->neurons[0][i].w);
    }
    for(unsigned i = 1; i < network->layerCount-1;i++)
    {
        for (unsigned j = 0; j < network->neuronPerLayer;j++)
        {
            free((network->neurons[i][j]).w);
        }
    }
    
    for (unsigned i = 0 ; i < network->layerCount-1;i++)
    {
        free(network->neurons[i]);
    }
    free(network->neurons);
    free(network);
    network = NULL;
}

double sigmoide(double s)
{
    return 1 / (1 + exp(-s));
}

void learn(struct neural_network *network, double *res)
{
    //Output
    unsigned n = network->layerCount;
    for (int i = 0; i < network->outputCount; i++) {
        network->neurons[n-1][i].delta = res[i] -
        sigmoide(network->neurons[n - 1][0].input);
    }
    

    //Last hidden Layer
    for (unsigned i = 0; i < network->neuronPerLayer;i++)
    {
        for (unsigned j = 0; j < network->outputCount; j++) {
            double in = sigmoide(network->neurons[n-2][i].input);
            double w = network->neurons[n-2][i].w[j];
            network->neurons[n-2][i].delta = in*(1-in)*
            (w*network->neurons[n-1][j].delta);
        }
        
    }

    //Hidden layers
    struct neuron **neurons = network->neurons;

    for (int i = n-3 ; i > 0 ; i--)
    {
        for (unsigned j = 0; j < network->neuronPerLayer;j++)
        {
            double tmp = 0;
            for (unsigned k = 0; k < network->neuronPerLayer;k++)
            {
                tmp += neurons[i+1][k].delta*neurons[i][j].w[k];
            }
            neurons[i][j].delta = tmp*sigmoide(neurons[i][j].input)*
                (1-sigmoide(neurons[i][j].input));
        }

    }
    //Input maj w
    for (unsigned i = 0; i < network->inputCount; i++)
    {
        for (unsigned j = 0; j < network->neuronPerLayer; j++)
        {
            neurons[0][i].w[j] += alpha*sigmoide(neurons[0][i].input)*
                neurons[1][j].delta;
        }
    }
    //Hidden layer
    for(unsigned i = 1; i < n - 2; i++)
    {
        for (unsigned j = 0; j < network->neuronPerLayer;j++)
        {
            for (unsigned k = 0; k < network->neuronPerLayer;k++)
            {
                neurons[i][j].w[k] += alpha*sigmoide(neurons[i][j].input)*
                    neurons[i+1][k].delta;
            }
        }
    }

    //Last hidden layer
    for (unsigned j = 0; j < network->neuronPerLayer;j++)
    {
        for (unsigned i = 0; i < network->outputCount; i++) {
            neurons[n-2][j].w[i] += alpha*sigmoide(neurons[n-2][j].input)*
            neurons[n-1][i].delta;
        }
        
    }

    resetNetwork(network);
}

void resetNetwork(struct neural_network *network)
{
    //Hidden layer
    for(unsigned i = 1; i < network->layerCount - 1; i++)
    {
        for (unsigned j = 0; j < network->neuronPerLayer;j++)
        {
            network->neurons[i][j].delta = 0;
            network->neurons[i][j].input = 0;
        }
    }

    //Output
    for (unsigned i = 0; i < network->outputCount; i++) {
        network->neurons[network->layerCount-1][i].delta = 0;
        network->neurons[network->layerCount-1][i].input = 0;
    }

}

void resetWeights(struct neural_network *network)
{
    //input layer
    for(unsigned i = 0; i < network->inputCount; i++)
    {
        for(unsigned j = 0; j < network->neuronPerLayer; j++)
        {
            network->neurons[0][i].w[j] = my_random();
        }
    }
    //hidden layers
    for(unsigned i = 1; i < network->layerCount - 2; i++)
    {
        for(unsigned j = 0; j < network->neuronPerLayer; j++)
        {
            for(unsigned k = 0; k < network->neuronPerLayer; k++)
            {
                network->neurons[i][j].w[k] = my_random();
            }
        }
    }
    //last hidden layer
    for(unsigned i = 0; i < network->neuronPerLayer; i++)
    {
        for (int j = 0; j < network->outputCount; j++) {
             network->neurons[network->layerCount - 2][i].w[j] = my_random();
        }
       
    }

}


int save(struct neural_network *network)
{
    FILE *f = fopen("save","w+");
    if (!f || !network) {
        fclose(f);
        return -1;
    }
    struct neuron **neurons = network->neurons;
    
    fprintf(f, "%u %u %u %u\n", network->inputCount, network->layerCount, network->neuronPerLayer, network->outputCount);
    //inputs
    for (unsigned i = 0; i < network->inputCount; i++) {
        fprintf(f, "[");
        for (unsigned j = 0; j < network->neuronPerLayer; j++) {
            fprintf(f,"%f ",neurons[0][i].w[j]);
        }
        fprintf(f, "]");
    }
    fprintf(f, "\n");
    
    //hiddens
    for (unsigned i = 1; i < network->layerCount - 2; i++) {
        for (unsigned j = 0; j < network->neuronPerLayer; j++) {
            fprintf(f, "[");
            for (unsigned k = 0; k < network->neuronPerLayer; k++) {
                fprintf(f, "%f ", neurons[i][j].w[k]);
            }
            fprintf(f, "]");
        }
        fprintf(f,"\n");
    }
    //last hidden
    for (unsigned i = 0; i < network->neuronPerLayer; i++)
        fprintf(f, "[%f ]", neurons[network->layerCount - 2][i].w[0]);

    for (int i = 0; i < network->neuronPerLayer; i++) {
        fprintf(f, "[");
        for (unsigned j = 0; j < network->outputCount; j++) {
            fprintf(f, "%f ", neurons[network->layerCount - 2][i].w[j]);
        }
        fprintf(f, "]");
        
    }
    char c = '\\';
    fprintf(f,"%s",(const char *)&c);
    fclose(f);
    
    printf("saved\n");
    
    return 1;
}

struct neural_network* loadNetwork()
{
    FILE *f = fopen("save", "r");
    if (!f) {
        fclose(f);
        return NULL;
    }
    char c;
    
    int index = 0;
    char **sizes = calloc(4, sizeof(int*));
    int inputCount = 0, layerCount = 0, neuronPerLayer = 0;

    
    while (( c = fgetc(f) ) != '\n') {
        if (c != ' ')
        {
            sizes[index] = realloc(sizes[index], sizeof(sizes[index]) + sizeof(char));
            if (!sizes[index]) {
                printf("lol\n");
            }
            char *c2 = malloc(sizeof(char)*2);
            c2[0] = c;
            c2[1] = '\0';
            sizes[index] = strcat(sizes[index], (const char*)c2);
            //free((void *)c2);
        }
        else
        {
            index++;
        }
    }
    
    struct neural_network *network = createNetwork(atoi(sizes[1]), atoi(sizes[0]), atoi(sizes[2]), atoi(sizes[3]));
    
    int i = 0, j = 0, k = 0;
    char *w = calloc(9, sizeof(char));
    index = 0;
    while( ( c = fgetc(f) ) != EOF && c != '\\')
    {
        switch (c) {
            case  ' ':
                network->neurons[i][j].w[k] = atof(w);
                w = calloc(9, sizeof(char));
                index = 0;
                k++;
                break;
            case '[':
                break;
            case ']':
                j++;
                k = 0;
                break;
            case '\n':
                i++;
                j = 0;
                break;
            default:
                w[index] = c;
                index++;
                break;
        }
    }
    fclose(f);
    return network;
    
}
