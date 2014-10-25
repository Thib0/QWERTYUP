#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "image_detection.h"


IplImage* load(char* image)
{
  IplImage* img = NULL; 
  
  //int x,y;

  img = cvLoadImage(image, CV_LOAD_IMAGE_GRAYSCALE);

  if (img == NULL)
  {
    //fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return NULL;
  }

    // call fonction to detection         ;
    int *lines_number=malloc(sizeof(int));
    int **ptr_line = &lines_number;
    int size_lines_number = detect_line(img,lines_number,ptr_line);
   
   printf("jeajea test"); 
    // rect of all the chars
    struct rect_char *chars= malloc(sizeof(struct rect_char));
    struct rect_char **ptr_rect = &chars;
    int size_rect_char =detect_char(img,lines_number,size_lines_number,chars
    ,ptr_rect);
    
    for (int i = 0 ; i < size_lines_number; i++)
	color_line(img,lines_number[i]);
   
    for (int j = 0 ; j <size_rect_char ; j++)
    {
	
	color_column(img,chars[j]);
    }

    printf("nbr ligne : %i",size_lines_number);
   printf("nbr char : %i", size_rect_char);

    if (size_rect_char != 0)
      printf("success\n");
    
     free(lines_number);
     free(chars);


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
