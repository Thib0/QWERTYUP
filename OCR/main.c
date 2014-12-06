#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "neural_network.h"
#include <time.h>
#include "image_treatment.h"
void xor();
void setInput(int a, int b, struct neural_network *network)
{
	network->neurons[0][0].input = a;
	network->neurons[0][1].input = b;
	network->neurons[0][2].input = 1;
}

size_t max(double *out, size_t index)
{
	size_t max = 0;
	for (size_t i = 0; i < 128; i++) {
		if (out[i] > out[max] && i != index) {
			max = i;
		}
	}
	return max;
}
/*
   void setImageInput(struct neural_network network, IplImage img)
   {
   for (int i = 0; i < img->width; i++) {
   for (int j = 0; j < img->height; j++) {
   network->neurons[0][j*img->height + i].input = CV_IMAGE_ELEM(img, uchar, j, i)/255;
   }
   }
   }*/

int main (int argc, char* argv[])

{

	if (argc == 1)
	{
		srand(time(NULL));
		//struct neural_network *network = createNetwork(3, 3, 2, 1);
		//setInput(0,1,network);
		//save(network);
		//getOutput(network);
		//return 0;
		/* IplImage a = load("chars/a.png");
		   IplImage *b = load("chars/b.png");

		   double *out = malloc(1);
		   size_t m;
		   for (int i = 0; i < 100; i++) {
		   setImageInput(network, a);
		   do {
		   out = getOutput(network);
		   learn(network, 'a');
		   m = max(out, 'a');
		   } while (out['a'] < 0.7 || out[m] > 0.6);
		   printf("a done\n");
		   setImageInput(network, b);
		   do {
		   out = getOutput(network);
		   learn(network, 'b');
		   } while (out['b'] < 0.7 || out[m] > 0.6);
		   printf("%i\n", i);
		   }


		   setImageInput(network, a);
		   out = getOutput(network);
		   for (int i = 0; i < 128; i++) {
		   printf("%c: %f\n", (char)i, out[i]);

		   save(network);*/
		_Exit(0);

	}

	else if (argc == 2)
	{
		IplImage *img = load(argv[1]);
		if(img != NULL)
		{


			//img = treatment(img);
			int *nb_char = malloc(sizeof(int));
			struct rect_char *chars = detection(img,nb_char);
			printf("nb char %i",*nb_char);

			const char* window_title = "Perfect Image";
			cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
			cvShowImage (window_title, img);
			cvWaitKey(0);
			char path[] = "result.bmp";
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
	}

	void xor()
	{
		printf("wtf\n");
		struct neural_network *network = createNetwork(3, 4, 3, 1);
		printf("lawl\n");
		int loopCount = 0, count = 1, proceed = 0;


		double *out00 = (double *)malloc(1), *out01 = (double *)malloc(1),  *out10 = (double*)malloc(1), *out11 = (double*)malloc(1);
		do
		{
			do
			{
				free(out00);
				free(out01);
				free(out10);
				free(out11);
				loopCount++;
				setInput(1,1, network);
				out11 = getOutput(network);
				learn(network, 1);
				setInput(0,0, network);
				out00 = getOutput(network);
				learn(network, 1);
				setInput(0,1,network);
				out01 = getOutput(network);
				learn(network, 0);
				setInput(1,0, network);
				out10 = getOutput(network);
				learn(network, 0);

				printf("0xor0: %f 0xor1: %f 1xor0: %f 1xor1: %f\n",out00[0], out01[0],
						out10[0], out11[0]);

			}while ((out00[0] > 0.05 || out01[0] < 0.95 || out10[0] < 0.95 ||
						out11[0] > 0.05) && loopCount < 200000);
			proceed = loopCount == 200000;
			if(proceed)
			{
				//printf("creating new network\n");
				resetWeights(network);
				//printf("weights reset");
				count++;
				loopCount = 0;
			}
		}while(proceed);

		save(network);
		freeNetwork(network);
		network = loadNetwork();
		printf("Network freed.\n");
		printf("Loop count : %i\n",(count - 1)*200000 + loopCount);
		printf("Everything went fine. Exiting...\n");
	}
