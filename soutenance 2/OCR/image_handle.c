#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "image_detection.h"
#include "image_treatment.h"
#include "char_treatment.h"

IplImage* load(char* image)
{
    IplImage* img = NULL;

    //int x,y;

    img = cvLoadImage(image, CV_LOAD_IMAGE_COLOR);

    if (img == NULL)
    {
        //fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
        return NULL;
    }

    return img;


}

IplImage * treatment(IplImage *img)
{		
    const char* window_title = "Perfect Image";
    cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
    cvShowImage (window_title, img);
    cvWaitKey(0);
    cvDestroyAllWindows();
    gray_s(img); 
    cvShowImage ("Grayscale", img);
    cvWaitKey(0);
    cvDestroyAllWindows();

    //contraste
    int mat[9] = {0,-1,0,-1,5,-1,0,-1,0};
    //elimination bruit
    int mat2[9] = {1,1,1,1,5,1,1,1,1};
    // elimination bruit
    matrix(img,mat2,13);
    cvShowImage ("Noise Elimination", img);
    cvWaitKey(0);
    cvDestroyAllWindows();

    //contraste
    matrix(img,mat,1);
    cvShowImage ("Contraste", img);
    cvWaitKey(0);
    cvDestroyAllWindows();


    median(img);  
    cvShowImage ("Median", img);
    cvWaitKey(0);
    cvDestroyAllWindows();


    binarization(img);
    cvShowImage ("Binarization", img);
    cvWaitKey(0);
    cvDestroyAllWindows();


    return img;

}

struct rect_char * detection(IplImage *img, int *nb_char)
{

    return detect(img,nb_char);

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
            if (CV_IMAGE_ELEM(img1, uchar, j, i) == CV_IMAGE_ELEM(newImage,
                        uchar, j, i))
            {
                n++;
            }
        }
    }
    return ((double) n/(img1->width * img1->height))*100;
}
