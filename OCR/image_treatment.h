#ifndef _IMAGE_TREATMENT_H
#define _IMAGE_TREATMENT_H
#include "opencv/cv.h"
#include "opencv/highgui.h"

int set_pixel(IplImage *img,int y,int x,int value);
uchar get_pixel(IplImage *img,int x,int y,int color);
int gray_s(IplImage *img);
int binarization(IplImage *img);
int matrix(IplImage *img, int *m_cov, int val_m_cov);
#endif
