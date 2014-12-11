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
#define ALPHA 0.3
#define BETA 0.9

void learnAlphabet(neural_network *network);

char *alphabet = 
" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

int main (int argc, char* argv[])
{

	if (argc == 1)
	{

        int layerCount[4] = {20*20, 20*20*2, 150, 128};
        neural_network *network = createNetwork(4, layerCount,BETA, ALPHA);
        learnAlphabet(network);
        //saveNetwork(network);
        //freeNetwork(network);
        _Exit(0);
	}

	else if (argc == 2)
	{
		IplImage *img = load(argv[1]);
		if(img != NULL)
		{


            img = treatment(img);
            int nb_char;
            char path[11] = "result.bmp";
            struct rect_char *chars = learning_detection(img, &nb_char);
            //printf("jeajea test: %i %i\n", chars[0].x, chars[nb_char-1].x);

            cvSaveImage(&path[0], img, NULL);
            cvShowImage ("Chars detection", img);
            cvWaitKey(0);
            cvDestroyAllWindows();

            neural_network *network = loadNetwork();
            char *str = getString(chars, nb_char, img, network);
            printf(" ");
            printf("%s%s\n","\x1B[31m", str);
            printf("%s", "\x1B[37m");
            saveToFile(str, "Result OCR");
            freeNetwork(network);
            free(chars);
            free(str);
            cvReleaseImage(&img);
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
    charArray[(int)c] = 1;
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
    double **input = malloc((nb_char + 1)* sizeof(double *));
    input[0] = spaceImage();

    for (int i = 1; i < nb_char + 1; i++) {
        input[i] = imageArray(create_char(img, rect_chars[i-1]));
    }
    double **output = malloc(sizeof(double *) * 128);

    int i = 0;
    for (char *c = alphabet; *c; c++, i++) {
        output[i] = getCharOutput(*c);
    }

    int num_iter = 5000;
    int report_per_iter = 100;
    int dataRows = nb_char + 1;

    for (int i = 0; i < num_iter; i++) {
        int row = i % dataRows;

        runForward(network, input[row]);
        double err = evalError(network, output[row]);

        if (err > 0.1)
        {
            runBackward(network, input[row], output[row], 0);
        }
        if (!(i % report_per_iter))
        {
            err = evalError(network, output[row]);
            printf("iter = %i err = %f char = %c\n", i, err, alphabet[row]);
        }

    }

}
















