#ifndef _IMAGE_DETECTION_H_
#define _IMAGE_DETECTION_H_
#include "opencv/cv.h"
#include "opencv/highgui.h"




int detect_line(IplImage *img, int *lines_number );
int line_value (IplImage *img,int y);
int color_line(IplImage *img, int y);
struct rect_char
{
  int x;
  int y;
  int width;
  int height;
};
int detect_char (IplImage *img,int line_numbers[],int line_numbers_size,
                 struct rect_char chars[]);
int column_value(int y1, int y2,int x, IplImage *img);
int color_column(IplImage *img,struct rect_char chars);

#endif
