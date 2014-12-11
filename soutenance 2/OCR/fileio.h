#ifndef _FILE_IO_
#define _FILE_IO_
#include "image_detection.h"
#include "char_treatment.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "neural_network.h"

char *getString(struct rect_char *rect_chars, size_t size, IplImage *img,
        neural_network *network);
int saveToFile(char *string, char *fileName);
double *imageArray(IplImage *img);

#endif
