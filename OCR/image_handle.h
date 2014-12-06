#ifndef _IMAGE_HANDLE_H_
#define _IMAGE_HANDLE_H_
#include "opencv/cv.h"
#include "opencv/highgui.h"

/*
   Loads an image at path with given path
   by converting it to a greyscale image.
   Returns NULL if image doesn't exist
   or couldn't be loaded. Returned image
   has only one component.
   */

IplImage* load(char* image);

/*
   Returns a percentage of comparison of
   the two given images. Images should be
   with one component. Returns -1 if one of
   the images is NULL. img2 is scaled to the
   size of img1 for comprison.
   */

double compare(IplImage *img1, IplImage *img2);

/*
Make treatment to the image and make the image perfect
*/
IplImage * treatment(IplImage *img);

struct rect_char * detection(IplImage *img, int *nb_char);

#endif
