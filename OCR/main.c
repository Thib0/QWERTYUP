#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "neural_network.h"
#include <time.h>

int main (int argc, char* argv[])
{
    srand(time(NULL));
    
    /*struct neuron* network = malloc(sizeof(struct neuron));
    network->connections = malloc(sizeof(struct neural_connection) * 2);
    
    network->connectionsCount = 2;
    network->type = input;
    struct neuron *neuron1 = malloc(sizeof(struct neuron));
    neuron1->connectionsCount = 0;
    neuron1->type = output;
    struct neuron *neuron2 = malloc(sizeof(struct neuron));
    neuron2->connectionsCount = 0;
    neuron2->type = output;
    
    network->connections[0].start = network;
    network->connections[0].end = neuron1;
    network->connections[1].start = network;
    network->connections[1].end = neuron2;
     
    initWeights(network);
    displayWeights(network);
    */

  /*  struct neural_network *network = createNetwork(3, 2, 2);
    printf("network created\n");
    network->neurons[0][0].input = 1;
    network->neurons[0][1].input = 0;
    printf("input set\n");
    double out = getOutput(network);
    printf("%f\n",out);
*/    
    if(argc < 2)
    {
        return EXIT_SUCCESS;
    }
    IplImage *img = load(argv[1]);
    if(img != NULL)
    {
        const char* window_title = "Perfect Image";
        cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
        cvShowImage (window_title, img);
        cvWaitKey(0);
        char path[] = "result.bmp";
        cvSaveImage(&path[0], img, NULL);
        cvDestroyAllWindows();
        cvReleaseImage(&img);
    }
    else
    {
        printf("Error loading image.\n");
    }
  return EXIT_SUCCESS;
}
