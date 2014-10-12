#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
int detect_line(IplImage *img, int *lines_number );
int line_value (IplImage *img,int y);
int color_line(IplImage *img, int y);


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
        if (color - 10 >= colorBackground || color > 190)
            {
                 CV_IMAGE_ELEM(img, uchar, y, x) = 255;         
            }
        else 
            {
                CV_IMAGE_ELEM(img, uchar, y, x) = 0;                
            }
   }
  }
    //img = checkNoise(img);
    int x2,y2,w,z,isPerfect,count = 0;
    //printf("%d\n",CV_IMAGE_ELEM(img, uchar, 0, 0));
    for (y2 = 0; y2 < img->height && y2 + 10 < img->height;y2 += 10)
        {
            //printf("y2 = %d\n",y2);
            for (x2 = 0; x2 < img->width && x2 + 10 < img->width ;x2 += 10)
                {  
                    isPerfect = 0;
                    count = 0;
                    //printf("x2 = %d\n",x2);
                    for (w = y2; w < y2 + 10; w++)
                       {
                            for (z = x2; z < x2 + 10; z++)
                            {
                                //printf("w = %d and z = %d\n",w,z);
                                if (CV_IMAGE_ELEM(img, uchar, w, z) != 255 && !isPerfect)
                                    { count++; }
                                //printf("count = %d\n", count);
                                if (w == y2 + 9 && z == x2 + 9 && count <= 3 && !isPerfect)
                                    {
                                        isPerfect++;
                                    }
                                if (isPerfect == 1)
                                    {
                                        //printf("w = %d and z = %d\n",w,z);
                                        w = y2;
                                        z = x2;
                                        isPerfect++;
                                        //printf("w = %d and z = %d\n",w,z);
                                    }
                                
                                if (isPerfect && CV_IMAGE_ELEM(img, uchar, w, z) != 255)
                                    {
                                        CV_IMAGE_ELEM(img, uchar, w, z) = 255;
                                    }
                            }
                       }
                }
        }

    
    // call fonction to detect lines                                                                                                            
    int *lines_number = malloc(1000);
    int size_lines_number = detect_line(img,lines_number);
    if (size_lines_number != 0)
      printf("succes");

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

int detect_line(IplImage *img, int *lines_number )
{

  int y,y0,y1,y2,size_lines_number = 0;
  for (y=1; y<( img->height - 1);y++)
    {
      y0 = line_value(img,y-1); // test y-1                                                                                                       
      y1 = line_value(img,y);   // test y                                                                                                         
      y2 = line_value(img,y+ 1); // test y+1                                                                                                      

      // printf("%i\n",y);                                                                                                                        

      if ((y1==0 && y2==1) || (y1==0 && y0==1))
	{

	  lines_number[size_lines_number] = y;
	  size_lines_number ++;
	}
    }

  for (int i = 0 ; i < size_lines_number;i++)
    {
      color_line(img,lines_number[i]);
    }

  return size_lines_number;
}

int line_value (IplImage *img,int y)
{
  int x;

  for (x = 0 ; x < img->width; x++)
    {
      // if black pixel return 1                                                                                                                  
      if (CV_IMAGE_ELEM(img,uchar,y,x) == 0)
	{
	  return 1;
	}
    }
  // if only white pixel, return 0                                                                                                              
  return 0;
}

int color_line (IplImage *img, int y)
{
  int x;
  for (x = 0 ; x < img->width; x++)
    {

      CV_IMAGE_ELEM(img,uchar,y,x)=0;
    }

  return 0;
}
