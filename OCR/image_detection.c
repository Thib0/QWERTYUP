#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "image_detection.h"



int detect_line(IplImage *img, int *lines_number,int **ptr )
{


    int y,y0,y1,y2,size_lines_number = 0;

    for (y=4; y<( img->height - 1);y++)
    {
        y0 = line_value(img,y-1);
        y1 = line_value(img,y);  
        y2 = line_value(img,y+ 1);             

        if ( y1==0 && y2==1)
        {
            if (size_lines_number != 0)
                lines_number =realloc_l(ptr,(size_lines_number+1)*sizeof(int)) ;

            lines_number[size_lines_number] = y;
            size_lines_number ++;
        }
        else if (y1==0 && y0==1)
        {
            lines_number =realloc_l(ptr,(size_lines_number+1)*sizeof(int))  ;

            lines_number[size_lines_number] = y;
            size_lines_number ++;

        }
    }


    return size_lines_number;
}


int line_value (IplImage *img,int y)
{
    int x;

    for (x = 0 ; x < img->width; x++)
    {
        // if black pixel return 1
        if (CV_IMAGE_ELEM(img,uchar,y,x) <50)
        {
            return 1;
        }
    }                                                                 
    return 0;
}


int color_line (IplImage *img, int y)
{
    int x;
    for (x = 0 ; x < img->width; x++)
    {

        CV_IMAGE_ELEM(img,uchar,y,x)=128;
    }

    return 0;
}

int detect_char (IplImage *img,int *lines_number,int line_numbers_size,
        struct  rect_char *chars,struct rect_char **ptr)
{

    // detection space 
    int white = 0;
    int first_char = 0;

    int i,x,x1,x2,x3,mem_x1,mem_x2,c=0,white_ref;

    for (i=0;i< line_numbers_size;i= i+2)
    {
        mem_x1 = -1;
        mem_x2 = -1;
        white = 0;
        first_char = 0;
        white_ref = 0.18 * (lines_number[i+1]-lines_number[i]);
        if (white_ref >10)
            white_ref = 10;

        for(x=1 ; x < img->width-1; x++)
        {

            x1 = column_value(lines_number[i],lines_number[i+1],x-1,img);
            x2 = column_value(lines_number[i],lines_number[i+1],x,img);
            x3 = column_value(lines_number[i],lines_number[i+1],x+1,img);


            if (x2 == 0 && x1 == 1)
            {
                mem_x2=x;	
            }

            if(mem_x2 != -1 && mem_x1 != -1)
            {
                if (c != 0)
                    chars = realloc_r(ptr,(c+1)*sizeof(struct rect_char));


                chars[c].y = lines_number[i];
                chars[c].x = mem_x1;
                chars[c].width = mem_x2 - mem_x1;
                chars[c].height = lines_number[i+1] - lines_number[i];
                c++;


                if (white > white_ref && c>0 && first_char == 1 )
                {
                    chars = realloc_r(ptr,(c+1)*sizeof(struct rect_char));

                    chars[c].y = lines_number[i];
                    chars[c].x = chars[c-2].x +chars[c - 2].width +2 ;
                    chars[c].width = white -3;
                    chars[c].height = lines_number[i+1]-lines_number[i];
                    c++;
                }
                white =0;
                first_char = 1;



                mem_x2 = -1;
                mem_x1 = -1;
            }


            if( x1 == 0 && x2 ==0 && x3 ==0)
            {
                white ++;	 
            }

            if (x2 == 0 && x3 == 1 )
            {
                mem_x1 = x;
            }


        }
    }

    return c;
} 

int column_value(int y1, int y2,int x, IplImage *img)
{
    int i;
    for(i=y1 +1 ; i<y2;i++)
    {
        // if black pixel return 1                                                
        if (CV_IMAGE_ELEM(img,uchar,i,x) < 50)
        {
            return 1;

        }

    }
    return 0;
}

int color_column(IplImage *img,struct rect_char chars)
{
    for (int y = chars.y ; y < chars.y + chars.height; y++)
    {
        CV_IMAGE_ELEM(img,uchar,y,chars.x) = 128;
        CV_IMAGE_ELEM(img,uchar,y,chars.x+ chars.width) = 128;
    }

    return 0;
}

int* realloc_l(int **ptr, size_t taille)
{
    int *ptr_realloc = realloc(*ptr, taille);

    if (ptr_realloc != NULL)
        *ptr = ptr_realloc;

    return ptr_realloc;
}
struct rect_char* realloc_r(struct rect_char **ptr, size_t taille)
{
    struct rect_char *ptr_realloc = realloc(*ptr, taille);

    if (ptr_realloc != NULL)
        *ptr = ptr_realloc;

    return ptr_realloc;
}

int color(IplImage *img,int *lines_number, struct rect_char *chars, int
        size_l,int size_c)
{

    for (int i = 0 ; i < size_l; i++)
        color_line(img,lines_number[i]);

    for (int j = 0 ; j <size_c ; j++)
    {

        color_column(img,chars[j]);
    }

    return 0;

}
