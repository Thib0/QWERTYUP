#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neural_network.h"
#include <math.h>
#include <unistd.h>
#include <string.h>

double my_random()
{
    return (double)(rand())/(RAND_MAX/2) - 1;
}

double sigmoide(double s)
{
    return 1 / (1 + exp(-s));
}

neural_network *createNetwork(unsigned nLayers, int *layersize, double beta,
        double alpha)
{
    neural_network *nn = (neural_network *)malloc(sizeof(neural_network));
    nn->alpha = alpha;
    nn->beta = beta;

    //    set no of layers and their sizes
    nn->nLayers = nLayers;
    nn->layersize = (int *)malloc(nLayers * sizeof(int));

    for(unsigned i=0; i<nLayers; i++)
        nn->layersize[i] = layersize[i];

    nn->output = (double **)malloc(nLayers * sizeof(double *));
    for(unsigned i=0; i<nLayers; i++)
        nn->output[i] = (double *)malloc(layersize[i] * sizeof(double));

    nn->delta = (double **)malloc(nLayers * sizeof(double *));
    for(unsigned i=1; i<nLayers; i++)
        nn->delta[i] = (double *)malloc(layersize[i] * sizeof(double));

    nn->weight = (double ***)malloc(nLayers * sizeof(double **));
    for(unsigned i=1; i<nLayers; i++)
        nn->weight[i] = (double **)malloc(layersize[i] * sizeof(double *));

    for(unsigned i=1; i<nLayers; i++)
        for(int j=0; j<layersize[i]; j++)
            nn->weight[i][j] = (double *)malloc((layersize[i-1] + 1) * 
                    sizeof(double));

    nn->deltaweight_old = (double ***)malloc(sizeof(double **) * nLayers);
    for(unsigned i=1; i<nLayers; i++)
        nn->deltaweight_old[i] = (double **)malloc(layersize[i] * 
                sizeof(double *));
    for(unsigned i=1; i<nLayers; i++)
        for(int j=0; j<layersize[i]; j++)
            nn->deltaweight_old[i][j] = (double *)malloc((layersize[i-1] + 1) *
                    sizeof(double));

    srand((unsigned)(time(NULL)));
    for(unsigned i=1; i<nLayers; i++)
        for(int j=0; j<layersize[i]; j++)
            for(int k=0; k<layersize[i-1]+1; k++)
                nn->weight[i][j][k] = my_random();

    for(unsigned i=1; i<nLayers; i++)
        for(int j=0; j<layersize[i]; j++)
            for(int k=0; k<layersize[i-1]+1; k++)
                nn->deltaweight_old[i][j][k] = 0.0;

    return nn;
}


void freeNetwork(neural_network *nn)
{
    if (!nn)
        return;

    int i;
    for(i=0; i<nn->nLayers; i++)
        free(nn->output[i]);
    free(nn->output);
    nn->output = NULL;

    for(i=1; i<nn->nLayers; i++)
        free(nn->delta[i]);
    free(nn->delta);
    nn->delta = NULL;

    for(i=1; i<nn->nLayers; i++)
        for(int j=0; j<nn->layersize[i]; j++)
            free(nn->weight[i][j]);
    for(i=1; i<nn->nLayers; i++)
        free(nn->weight[i]);
    free(nn->weight);
    nn->weight = NULL;

    for(i=1; i<nn->nLayers; i++)
        for(int j=0; j<nn->layersize[i]; j++)
            free(nn->deltaweight_old[i][j]);
    for(i=1; i<nn->nLayers; i++)
        free(nn->deltaweight_old[i]);
    free(nn->deltaweight_old);
    nn->deltaweight_old = NULL;

    free(nn->layersize);
    nn->layersize = NULL;
}

double getOutput(neural_network *nn, int i)
{
    return nn->output[nn->nLayers-1][i];
}

void runForward(neural_network *nn, double *in)
{
    double sum;
    int i;

    for(i=0; i<nn->layersize[0]; i++)
        nn->output[0][i] = in[i];

    for(i=1; i<nn->nLayers; i++) {
        for(int j=0; j<nn->layersize[i]; j++) {
            sum = 0.0;
            for(int k=0; k<nn->layersize[i-1]; k++)
                sum += nn->output[i-1][k] * nn->weight[i][j][k];
            sum += nn->weight[i][j][nn->layersize[i-1]];
            nn->output[i][j] = sigmoide(sum);
        }
    }
}

void runBackward(neural_network *nn, double *in, double *desired, int forward)
{
    int nLayers = nn->nLayers;
    double **output = nn->output;
    int *layersize = nn->layersize;
    double ***weight = nn->weight;
    double ***deltaweight_old = nn->deltaweight_old;
    double **delta = nn->delta;
    double beta = nn->beta;
    double alpha = nn->alpha;

    double sum;
    int i;

    //    update output values for each neuron
    if (forward)
        runForward(nn, in);


    //    find delta for output layer
    for(i=0; i<layersize[nLayers-1]; i++)
        delta[nLayers-1][i] = output[nLayers-1][i] * (1 - output[nLayers-1][i])
            * (desired[i] - output[nLayers-1][i]);

    //    find delta for hidden layers
    for(i=nLayers-2; i>0; i--) {
        for(int j=0; j<layersize[i]; j++) {
            sum = 0.0;
            for(int k=0; k<layersize[i+1]; k++)
                sum += delta[i+1][k] * weight[i+1][k][j];
            delta[i][j] = output[i][j] * (1 - output[i][j]) * sum;
        }
    }

    //    apply momentum ( does nothing if alpha=0 )
    for(i=1; i<nLayers; i++) {
        for(int j=0; j<layersize[i]; j++) {
            for(int k=0; k<layersize[i-1]; k++)
                weight[i][j][k] += alpha * deltaweight_old[i][j][k];
            weight[i][j][layersize[i-1]] += alpha * 
                deltaweight_old[i][j][layersize[i-1]];
        }
    }

    //    adjust weights usng steepest descent
    for(i=1; i<nLayers; i++) {
        for(int j=0; j<layersize[i]; j++) {
            for(int k=0; k<layersize[i-1]; k++) {
                deltaweight_old[i][j][k] = beta * delta[i][j] * output[i-1][k];
                weight[i][j][k] += deltaweight_old[i][j][k];
            }
            deltaweight_old[i][j][layersize[i-1]] = beta * delta[i][j];
            weight[i][j][layersize[i-1]] += 
                deltaweight_old[i][j][layersize[i-1]];
        }
    }
}

double evalError(neural_network *nn, double *desired)
{
    int nLayers = nn->nLayers;
    double **output = nn->output;
    double error = 0.0;
    for(int i=0; i<nn->layersize[nLayers-1]; i++){
        error += (desired[i] - output[nLayers-1][i]) * (desired[i] - 
                output[nLayers-1][i]);
    }
    return error / 2;
}

int saveNetwork(neural_network *network)
{
    FILE *f = fopen("save/values", "w+");
    if (!f)
        return 0;

    fprintf(f, "%i\n", network->nLayers); // nLayers
    for (int i = 0; i < network->nLayers; i++)
        fprintf(f, "%i\n", network->layersize[i]); // layersize
    fprintf(f, "%f\n", network->beta); // beta
    fprintf(f, "%f\n", network->alpha); // alpha
    fclose(f);

    /*
    //  Weights
    */

    f = fopen("save/weight", "w+");
    if (!f)
        return 0;


    for (int i = 1; i < network->nLayers; i++) {
        for (int j = 0; j < network->layersize[i]; j++) {
            fprintf(f, "[");
            for (int k = 0; k < network->layersize[i-1] + 1; k++) {
                fprintf(f, "%f ", network->weight[i][j][k]);
            }
            fprintf(f, "]");
        }
        fprintf(f, "\n");
    }
    fprintf(f,"\\");
    fclose(f);

    return 1;
}

neural_network *loadNetwork()
{
    FILE *f = fopen("save/values", "r");
    if (!f)
        return NULL;
    char *nLayers = calloc(2, sizeof(char));
    char c;
    for (int i = 0, c = fgetc(f); c != '\n' && c != EOF; i++, c = fgetc(f)) {
        nLayers[i] = c;
    }

    char **layersize = calloc(atoi(nLayers), sizeof(char *));

    for (int i = 0; i < atoi(nLayers); i++) {
        layersize[i] = calloc(4, sizeof(char));
    }

    int n = 0;
    int i = 0;
    while (n < atoi(nLayers)) {
        c = fgetc(f);
        if(c == '\n')
        {
            i = 0;
            n++;
        }
        else
        {
            layersize[n][i] = c;
            i++;
        }
    }

    for (int i = 0; i < atoi(nLayers); i++) {
        // printf("%i ",atoi(layersize[i]));
    }

    char *beta = calloc(9, sizeof(char));
    for (int i = 0, c = fgetc(f); c != '\n'; c = fgetc(f), i++) {
        beta[i] = c;
    }

    char *alpha = calloc(9, sizeof(char));
    for (int i = 0, c = fgetc(f); c != '\n'; c = fgetc(f), i++) {
        alpha[i] = c;
    }

    int *layersizei = malloc(sizeof(int) * atoi(nLayers));

    for (int i = 0; i < atoi(nLayers); i++) {
        layersizei[i] = atoi(layersize[i]);
        //printf("%i ", layersizei[i]);
    }


    fclose(f);

    neural_network *network = createNetwork(atoi(nLayers), layersizei, 
            atof(beta), atof(alpha));

    f = fopen("save/weight", "r");

    if (!f)
        return NULL;



    int  j = 0, k = 0, index = 0;
    i = 1;
    char *w = calloc(9, sizeof(char));

    while( ( c = fgetc(f) ) != EOF && c != '\\')
    {
        switch (c) {
            case  ' ':
                network->weight[i][j][k] = atof(w);
                free(w);
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

    return network;

}
