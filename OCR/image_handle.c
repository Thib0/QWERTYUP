#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"

IplImage* load(char* image)
{
  IplImage* img = NULL; 
  
  int x,y;

  img = cvLoadImage(image, CV_LOAD_IMAGE_GRAYSCALE);

  if (img == NULL)
  {
    //fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return NULL;
  }
  int colorBackground = CV_IMAGE_ELEM(img, uchar, 0, (0 * 3));
  //int i = img->nChannels;
  for (y = 0; y < img->height;y++)
  {
    for (x = 0; x < img->width;x++)
    {  
        int color = CV_IMAGE_ELEM(img, uchar, y, x);
        if (color - 10 >= colorBackground || color > 175)
            {
                 CV_IMAGE_ELEM(img, uchar, y, x) = 255;         
            }
        else 
            {
                CV_IMAGE_ELEM(img, uchar, y, x) = 0;                
            }
   }
  }
    return img;

}

double compare(IplImage *img1, IplImage *img2)
{
    if (img1 == NULL || img2 == NULL) {
        return -1;
    }
    CvSize size;
    size.width = img1->width;
    size.height = img1->height;
    IplImage *newImage = cvCreateImage(size, 8, 1);
    if (img1->width != img2->width || img1->height != img2->height) {
        cvResize(img2, newImage, CV_INTER_LINEAR);
    }else{
        newImage = img2;
    }
    unsigned long long n = 0;
    for(int i = 0; i < img1->width; i++)
    {
        for(int j = 0; j < img1->height; j++)
        {
            if (CV_IMAGE_ELEM(img1, uchar, j, i) == CV_IMAGE_ELEM(newImage, uchar, j, i))
            {
                n++;
            }
        }
    }
    return ((double) n/(img1->width * img1->height))*100;
}




