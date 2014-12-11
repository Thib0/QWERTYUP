#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"
#include "image_detection.h"
#include "char_treatment.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "neural_network.h"
#define tresh 0.07
double *imageArray(IplImage *img)
{
    double *array = malloc(sizeof(double)*20*20);
    for (int i = 0; i < 20*20; i++)
    {
        array[i] = 1 - get_pixel(img, i%20, i/20, 0) / 255;
    }
    return array;
}

char getChar(double *char_data, neural_network *network)
{
    runForward(network, char_data);
    int max = 0;
    for (int i = 0; i < 128; i++)
    {
        if(getOutput(network, max) < getOutput(network, i))
            max = i;
    }
    free(char_data);
    //printf("max: %c - output: %f\n",(char)max, getOutput(network, max));
    return (char) (getOutput(network, max) > tresh) ? max : '?';
}

char *getString(struct rect_char *rect_chars, size_t size, IplImage
        *img, neural_network *network)
{
    unsigned newLineCount = 0;
    /*for (int i = 0; i < size; i++) {
      if (rect_chars[i].section) {
      newLineCount++;
      }
      }*/
    char *chars = malloc(sizeof(char) * (size + newLineCount));

    for (unsigned i = 0, j = 0; i < size + newLineCount; i++, j++)
    {
        //printf("%i\n", i);
        /*if (rect_chars[j].section)
          {
          chars[i] = '\n';
          i++;
          }*/
        chars[i] = getChar(imageArray(create_char(img, rect_chars[j])),
                network);
    }

    return chars;
}

int saveToFile(char *string, char *fileName)
{
    FILE *f = fopen(fileName, "w+");
    if (!f)
        return 0;

    fprintf(f, "%s", string);

    fclose(f);
    return 1;
}
