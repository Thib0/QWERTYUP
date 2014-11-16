#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "image_detection.h"
#include "image_treatment.h"

int set_pixel(IplImage *img,int y,int x,int value)
{
	
	img->imageData[img->widthStep*y + 3*x ]=value;
	img->imageData[img->widthStep*y + 3*x + 1]=value;
	img->imageData[img->widthStep*y + 3*x + 2]=value;


	return 0;
}

uchar get_pixel(IplImage *img,int x,int y,int color)
{
	//printf("test get_pixel : %u\n",img->imageData[img->widthStep*y + 3*x +color] );
	return img->imageData[img->widthStep*y + 3*x + color];
}
