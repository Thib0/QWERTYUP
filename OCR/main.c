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

    struct neural_network *network = createNetwork(3, 2, 2);
    printf("Network created\n");
    network->neurons[0][0].input = 0; 
    network->neurons[0][1].input = 1;
    printf("Input set\n");
    double out = getOutput(network);
    printf("Output : %f\n",out);
    freeNetwork(network);
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
