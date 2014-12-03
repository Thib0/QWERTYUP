#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_treatment.h"
#include "char_treatment.h"
#include "image_detection.h"
#include "image_handle.h"


IplImage * create_char(IplImage *img, struct rect_char chars)
{
	IplImage *result = cvCreateImage(cvSize(chars.width,chars.height),8,3);
	int x1 = 0,y1 =0;
	for (int y=chars.y ; y < (chars.y + chars.height); y++)
	{	
		x1 = 0;
		for(int x= chars.x ; x < (chars.x + chars.width); x++ )
		{
			set_pixel(result,y1,x1,get_pixel(img,x,y,0));
			x1++;
		
		}
		y1++;
	}

	printf("jeajea test");
	if (chars.width != 20 || chars.height != 20 )
	{
		IplImage * new_result = cvCreateImage(cvSize(20,20),8,3);
		cvResize(result,new_result, CV_INTER_LINEAR);
		return new_result;
	}

	return result;
}
