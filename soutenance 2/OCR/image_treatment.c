#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "image_detection.h"
#include "image_treatment.h"
#include <math.h>

int set_pixel(IplImage *img,int y,int x,int value)
{

    img->imageData[img->widthStep*y + 3*x ]=value;
    img->imageData[img->widthStep*y + 3*x + 1]=value;
    img->imageData[img->widthStep*y + 3*x + 2]=value;


    return 0;
}

uchar get_pixel(IplImage *img,int x,int y,int color)
{
    // color 0 pour r , 1 pour g , 2 pour b
    return img->imageData[img->widthStep*y + 3*x + color];
}

int gray_s(IplImage *img)
{
    uchar r,g,b,gray;
    for(int x = 0 ; x < img->width ; x++)
    {
        for (int y = 0; y < img->height ; y++)
        {
            r = get_pixel(img,x,y,0);
            g = get_pixel(img,x,y,1);
            b = get_pixel(img,x,y,2);

            gray = 0.299*r + 0.587*g + 0.114*b;
            set_pixel(img,y,x,gray);
        }
    }
    return 0;
}

int binarization(IplImage *img)
{
    uchar bound = otsu_bina(img);

    for (int x = 0; x < img->width; x++)
    {
        for(int y = 0 ; y < img->height; y++)
        {
            if (get_pixel(img,x,y,0) > bound )
                set_pixel(img,y,x,255);
            else
                set_pixel(img,y,x,0);

        }
    }

    return 0;
}


int matrix(IplImage *img,int *m_cov,int val_m_cov)
{
    IplImage *ref = cvCreateImage(cvSize(img->width,img->height),8,3);
    for(int k = 0 ; k < img->height ; k++)
    {
        for(int l = 0; l < img->width ; l++)
        {
            set_pixel(ref,k,l,get_pixel(img,l,k,0));

        }

    }
    int value,cpt;

    for(int y = 2 ; y < img->height -2; y++)
    {

        for (int x =2 ; x < img->width -2; x++)
        {
            value = 0;
            cpt = 0;
            for (int  i = -1 ; i<2;i++)
            {
                for(int j = -1; j<2 ; j++)
                {
                    value += get_pixel(ref,(x+j),(y+i),0)*m_cov[cpt];

                    cpt++;
                }

            }

            if((value/val_m_cov) > 255)
                value = 255;
            else if ((value /val_m_cov) < 0)
                value = 0;

            set_pixel(img,y,x,value/val_m_cov);

        }
    }

    cvReleaseImage(&ref);
    return 0;

}


int median(IplImage *img)
{

    IplImage *save = cvCreateImage(cvSize(img->width,img->height),8,3);
    for(int k=0 ; k < img->height ; k++)
    {
        for(int l = 0 ; l < img->width ; l++)
        {
            set_pixel(save,k,l,get_pixel(img,l,k,0));

        }
    }


    int l[9]={0,0,0,0,0,0,0,0,0};
    int cpt = 0;
    for(int x = 2 ; x < img->width -2; x++)
    {

        for (int y =2 ; y < img->height -2; y++)
        {	


            cpt = 0;
            for (int  i = -1 ; i<2;i++)
            {
                for(int j = -1; j<2 ; j++)
                {
                    l[cpt]=get_pixel(save,(x+j),(y+i),0);
                    cpt++;
                }

            }

            set_pixel(img,y,x,median2(l,9));
        }	
    }
    cvReleaseImage(&save);
    return 0;

}

int median2(int *l,int size)
{
    for (int i = 0 ; i < size ; i++)
    {
        for (int j = 0 ; j < size-1 ; j++)
        {
            if (l[j]>l[j+1])
            {

                int a = l[j+1];
                l[j+1] = l[j];
                l[j] = a;
            }

        }
    }
    return l[size/2];
}



int otsu_bina(IplImage *img)
{

    int x,y,i,bound;
    int hist[256];

    // INIT tab
    for ( i = 0 ; i < 256; i++)
        hist[i] = 0;

    double max_sigma, sigma[256];
    double prob[256], omega[256], myu[256];


    for( y = 0 ; y < img->height ; y++)
    {
        for( x = 0 ; x < img->width ; x++)
        {
            hist[get_pixel(img,x,y,0)] += 1;
        }


    }

    for (i = 0 ; i < 256 ; i++)
    {
        prob[i] = (double)hist[i] / (img->width * img->height);

    }


    omega[0] = prob [0];
    myu[0] = 0.0;

    for( i = 1 ; i < 256 ; i++)
    {
        omega[i] = omega[i-1] + prob[i];
        myu[i] = myu[i-1] + i*prob[i];


    }

    bound = 0;
    max_sigma = 0.0;

    for( i = 0 ; i <255 ; i++)
    {
        if(omega[i] != 0.0 && omega[i] != 1.0)
        {
            sigma[i] = pow(myu[255]*omega[i] - myu[i],2) / (omega[i]*(1.0 
                        - omega[i]));	
        }
        else
        {
            sigma[i] = 0;

        }

        if (sigma[i] > max_sigma)
        {
            max_sigma = sigma[i];
            bound = i;

        }

    }


    return bound;

}
