#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "neural_network.h"
#include <time.h>
#include "image_treatment.h"
#include "fileio.h"
#include "image_detection.h"
#define alpha 0.3
#define beta 0.9

void learnAlphabet(neural_network *network);

char *alphabet = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

int main (int argc, char* argv[])
{

	if (argc == 1)
	{

        int layerCount[4] = {20*20, 20*20*2.5, 100, 128};
        neural_network *network = loadNetwork();
        learnAlphabet(network);
        saveNetwork(network);
        //freeNetwork(network);
        _Exit(0);
	}

	else if (argc == 2)
	{
		IplImage *img = load(argv[1]);
		if(img != NULL)
		{


			//img = treatment(img);
			int nb_char;
			struct rect_char *chars = learning_detection(img, &nb_char);
            char path[10] = "result.bmp";
            cvSaveImage(&path[0], img, NULL);
            int nLayers[3] = {20*20, 20*20*1.5, 128};
            neural_network *network = loadNetwork();
            char *str = getString(chars, nb_char, img, network);
            printf(" ");
            printf("%s\n", str);
            printf("%s\n", alphabet);
            saveToFile(str, "test OCR");
            freeNetwork(network);
            free(chars);
            free(str);
            return 0;


            printf("nb char %i", nb_char);

			const char* window_title = "Perfect Image";
			cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
			cvShowImage (window_title, img);
			cvWaitKey(0);
			cvSaveImage(&path[0], img, NULL);
			cvDestroyAllWindows();
			cvReleaseImage(&img);
			printf("Everything went fine. Exiting...\n");
			return 0;
		}
		else
		{
			printf("Error loading image.\n");
			return -1;
		}
	}
	else
	{
		printf("Argument issue. Exiting...\n");
		return -1;
	}
    return 0;
}


double *getCharOutput(char c)
{
    double *charArray = malloc(sizeof(double) * 128);
    for (int i = 0; i < c; i++) {
        charArray[i] = 0;
    }
    charArray[c] = 1;
    for (int i = c + 1; i < 128; i++) {
        charArray[i] = 0;
    }
    return charArray;
}

double *spaceImage()
{
    return calloc(20 * 20, sizeof(double));
}


void learnAlphabet(neural_network *network)
{


    int nb_char;
    IplImage *img = load("images/alphabet.png");
    struct rect_char *rect_chars = learning_detection(img, &nb_char);


    if (!img) {
        printf("error loading image lawl\n");
        return;
    }
    double **input = malloc((62 + 1)* sizeof(double *));
    input[0] = spaceImage();

    for (int i = 1; i < 62 + 1; i++) {
        input[i] = imageArray(create_char(img, rect_chars[i-1]));
    }
    double **output = malloc(sizeof(double *) * 128);

    int i = 0;
    for (char *c = alphabet; *c; c++, i++) {
        output[i] = getCharOutput(*c);
    }

    int num_iter = 30000;
    int report_per_iter = 1000;
    int dataRows = 63;

    for (int i = 0; i < num_iter; i++) {
        int row = i % dataRows;

        runForward(network, input[row]);
        double err = evalError(network, output[row]);

        if (err > 0.1)
            runBackward(network, input[row], output[row], 0);

        if (!(i % report_per_iter))
        {
            err = evalError(network, output[row]);
            printf("iter = %i err = %f\n", i, err);
        }

    }
    
    
/*
    for (int i = 0; i < 128; i++) {
        free(output[i]);
        if (i < 62) {
            free(input[i]);
        }
    }*/
    /*
    free(output);
    free(input);
    free(rect_chars);
    cvReleaseImage(&img);*/
}
















