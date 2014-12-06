#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "neural_network.h"
#include <time.h>
#include "image_treatment.h"
#include "fileio.h"

void my_xor()
{
    // XOR training data. last column is for outputs. 0 xor 0 = 0, 0 xor 1 = 1, ...
    double data[][3] = {
        0,    0,    0,
        0,    1,    1,
        1,    0,    1,
        1,    1,    0 };

    int layerSize[] = {2, 2, 1};
    int nLayers = sizeof(layerSize) / sizeof(layerSize[0]);
    int nDataRows = sizeof(data) / sizeof(data[0]);
    int nDataCols = sizeof(data) / nDataRows / sizeof(data[0][0]);
    int nInputs = layerSize[0];
    int nOutputs = nDataCols - nInputs;

    printf("numLayers=%i\nnumDataRows=%i\nnumDataCols=%i\nnumInputs=%i\nnumOutputs=%i\n", nLayers, nDataRows, nDataCols, nInputs, nOutputs);

    double beta = 0.9, alpha = 0.3, thresh = 0.0000001;
    neural_network *nn = createNetwork(nLayers, layerSize, beta, alpha);
    printf("lawl\n");

    // maximum no of iterations during training
    long num_iter = 2000000;
    long report_per_iter = 10000;
    long i;

    for (i=0; i<num_iter; i++)
    {
        int row = i % nDataRows;
        runBackward(nn, data[row], &data[row][nInputs]);

        double err = evalError(nn, &data[row][nInputs]);
        if(err < thresh) {
            printf("Trained in %ld iterations. Error = %f.\n", i, err);
            break;
        }
        if(i%report_per_iter == 0)
            printf("iter = %li, error = %f\n", i, err);
    }

    int j;
    for (i=0; i<nDataRows; i++)
    {
        printf("\nOutput for row %li: ", i);
        runForward(nn, data[i]);
        for(j=0; j<nOutputs; j++)
            printf("%i %f ", j, getOutput(nn, j));
    }
    saveNetwork(nn);
    freeNetwork(nn);

    printf("Network freed.\n");
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

double **charData;

void setUpCharData()
{
    charData = malloc(sizeof(double *) * 2);
    IplImage *a = load("../chars/a.png");
    IplImage *b = load("../chars/b.png");
    for (int i = 0; i < 20 * 20; i++) {
        charData[0] = malloc(sizeof(double) * 20 * 20);
        charData[1] = malloc(sizeof(double) * 20 * 20);
        charData[0][i] = get_pixel(a, i%a->width, i/a->height, 0) / 255;
        charData[1][i] = get_pixel(b, i%b->width, i/b->height, 0) / 255;
    }
}

double *resultArray(int c, size_t size)
{
    double *array = malloc(sizeof(double) * size);
    if (!array) {
        printf("error allocating array for: %c\n", c);
        _Exit(0);
    }
    for (int i = 0; i < size; i++) {
        array[i] = (i == c) ? 1 : 0;
    }
    return array;
}

void charDetect()
{
    setUpCharData();
    printf("char data set\n");
    int layerSize[] = {20 * 20, 20 * 20 * 1.5, 128};
    int nLayers = 3;
    int nDataRows = 2;
    int nDataCols = 20*20;
    int nInputs = 20*20;
    int nOutputs = 128;
    double beta = 5, alpha = 20, thresh = 0.1;
    
    
     printf(" numLayers=%i\n numDataRows=%i\n numDataCols=%i\n numInputs=%i\n numOutputs=%i\n", nLayers, nDataRows, nDataCols, nInputs, nOutputs);
    
    
    neural_network *nn = createNetwork(nLayers, layerSize, beta, alpha);
    
    printf("network creted\n");
    
    double *aResult = resultArray('a', 128);
    double *bResult = resultArray('b', 128);
    printf("results computed\n");
    
    double **results = malloc(sizeof(double *));
    results[0] = aResult;
    results[1] = bResult;
    
    // maximum no of iterations during training
    long num_iter = 15000;
    long report_per_iter = 100;
    long i;
    double err = 1;
    for (i=0; err > 0.3 && i < num_iter; i++)
    {
        int row = i % nDataRows;
        runBackward(nn, charData[row], results[row]);
        err = evalError(nn, results[row]);
        if (err <= 2.6) {
            nn->alpha = 0.1;
            nn->beta = 1.3;
        }
        if(err < thresh) {
            printf("Trained in %ld iterations. Error = %lf.\n", i, err);
            break;
        }
        if(i%report_per_iter == 0)
            printf("iter = %ld, error = %lf\n", i, err);
    }
    
    int j;
    runForward(nn, charData[0]);
    for (int i = 0; i < 128; i++) {
        printf("%c: %f - %f\n", i, getOutput(nn, i), results[0][i]);
    }
    scanf("%i", &j);
    
    runForward(nn, charData[1]);
    for (int i = 0; i < 128; i++) {
        printf("%c: %f - %f\n", i, getOutput(nn, i), results[1][i]);
    }
    
    
    /*
    for (i=0; i<nDataRows; i++)
    {
        printf("\nOutput for row %ld: ", i);
        runForward(nn, charData[i]);
        for(j=0; j<nOutputs; j++)
            printf("%d ", j, getOutput(nn, j));
    }*/
    
    freeNetwork(nn);
    
    printf("Network freed.\n");

}


int main (int argc, char* argv[])
{

    my_xor();
    return 0;
    
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

    //        else if (argc == 2)
    //        {
    //            IplImage *img = load(argv[1]);
    //            if(img != NULL)
    //            {
    //                const char* window_title = "Perfect Image";
    //                cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
    //                cvShowImage (window_title, img);
    //                cvWaitKey(0);
    //                char path[] = "result.bmp";
    //                cvSaveImage(&path[0], img, NULL);
    //                cvDestroyAllWindows();
    //                cvReleaseImage(&img);
    //                printf("Everything went fine. Exiting...\n");
    //                return 0;
    //            }
    //            else
    //            {
    //                printf("Error loading image.\n");
    //                return -1;
    //            }
    //        }
    //        else
    //        {
    //            printf("Argument issue. Exiting...\n");
    //            return -1;
    //        }
}
