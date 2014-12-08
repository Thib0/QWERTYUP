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
    chars = resize_char_x(img,chars);
    chars = resize_char_y(img,chars);
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


    if (chars.width != 20 || chars.height != 20 )
    {
        IplImage * new_result = cvCreateImage(cvSize(20,20),8,3);
        cvResize(result,new_result, CV_INTER_LINEAR);
        return new_result;
    }

    return result;
}

struct rect_char  resize_char_x(IplImage *img, struct rect_char chars)
{
    int i,j,test;
    for(i = chars.x ; i < chars.x + chars.width ; i++)
    {
        test = 0;
        for (j = chars.y ; j < chars.y+chars.height; j++)
        {
            if(get_pixel(img,i,j,0)==0)
            {

                test = 1;
            }

        }
        if (test)
            break;
    }

    chars.width = chars.width -(chars.x - i);
    chars.x = i;

    for(i = chars.x + chars.width ; i > chars.x ; i--)
    {
        test = 0;
        for(j = chars.y ; j < chars.y+chars.height; j++)
        {
            if(get_pixel(img,i,j,0)==0)
                test=1;
        }

        if(test)
            break;
    }

    chars.width = i - chars.x;

    return chars;
}

struct rect_char resize_char_y(IplImage *img, struct rect_char chars)
{
    int i,j,test;
    for(j = chars.y ; j < chars.y + chars.height ; j++)
    {
        test = 0;
        for(i = chars.x ; i < chars.x + chars.width; i++)
        {
            if (get_pixel(img,i,j,0) == 0)
                test = 1;


        }

        if (test)
            break;
    }



    chars.height = chars.height - (chars.y - j);
    chars.y = j;

    for(j = chars.y + chars.height; j > chars.y ; j--)
    {
        test = 0;
        for(i = chars.x ; i < chars.x + chars.width; i++)
        {
            if(get_pixel(img,i,j,0)==0)
                test = 1;
        }

        if(test)
            break;
    }

    chars.height = j - chars.y;
    return chars;
}
