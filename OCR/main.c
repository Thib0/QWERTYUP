#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include "image_handle.h"
#include <string.h>

int main (int argc, char* argv[])
{
    int i = 0;

    IplImage *img = load(argv[1]);
    if(img != NULL)
    {
        const char* window_title = "Perfect Image";
        cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
        cvShowImage (window_title, img);
        cvWaitKey(0);
        char path[] = "result.bmp";
        cvSaveImage(&path[0], img, NULL);
        cvDestroyAllWindows();
        cvReleaseImage(&img);
    }
    else
    {
        printf("Error loading image.\n");
    }
  return EXIT_SUCCESS;
}
