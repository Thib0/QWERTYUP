#ifndef _CHAR_TREATMENT_
#define _CHAR_TREATMENT_
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "image_treatment.h"
#include "image_detection.h"

IplImage * create_char(IplImage *img, struct rect_char chars);
struct rect_char  resize_char_x(IplImage *img, struct rect_char chars);
struct rect_char resize_char_y(IplImage *img, struct rect_char chars);

#endif
