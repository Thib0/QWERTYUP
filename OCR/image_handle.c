#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>

IplImage* load(int argc, char* argv[])
{
  IplImage* img = NULL; 
  
  int x,y;

  if (argc < 2)
  {
    //fprintf (stderr, "usage: %s IMAGE\n", argv[0]);
    return NULL;
  }
  img = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  if (img == NULL)
  {
    //fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return NULL;
  }
  int colorBackground = CV_IMAGE_ELEM(img, uchar, 0, (0 * 3));
  int i = img->nChannels;
  printf("%d\n",i);
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
