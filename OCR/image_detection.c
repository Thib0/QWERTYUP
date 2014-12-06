#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "image_detection.h"
#include "image_treatment.h"



struct rect_char * learining_detection(IplImage *img,int *nb_char)
{

	
	struct rect_char *chars = malloc(sizeof(struct rect_char));
	int *lines_number = malloc(sizeof(int));
	struct rect_char **ptr_rect = &chars;
	int **ptr_line = &lines_number;
	int size_lines_number = 0;
	int size_rect_char = 0;
	
	detect_line(img,lines_number,ptr_line,0,img->width,0);
	size_rect_char = detect_char(img,lines_number,size_lines_number,chars,ptr_rect,0,img->width,0);
	
	*nb_char = size_rect_char;
	free(lines_number);

	return chars;


}
struct rect_char * detect(IplImage *img, int * nb_char)
{
	
	
	struct rect_char *chars = malloc(sizeof(struct rect_char));
	int *lines_number = malloc(sizeof(int));
	struct rect_char **ptr_rect = &chars;
	int **ptr_line = &lines_number;
	int nb_bloc = 0;
	int *bloc = malloc(sizeof(int));
	int ** ptr_bloc = &bloc;
	nb_bloc = detect_bloc(img,bloc,ptr_bloc);
	int size_lines_number = 0;
	int size_rect_char = 0;
	int old_size_l;
	int old_size_c;

	for (int i = 0 ; i < nb_bloc ; i = i + 2)
	{

		old_size_l = size_lines_number;
		old_size_c = size_rect_char;
		size_lines_number =
		detect_line(img,lines_number,ptr_line,bloc[i],bloc[i+1],old_size_l);

		size_rect_char = detect_char(img,lines_number,size_lines_number,chars,ptr_rect,bloc[i],bloc[i+1],old_size_c);
	}
	
	//printf("nb chars : %i\n",size_rect_char);
	color(img,size_rect_char,chars);

	*nb_char = size_rect_char;

	free(lines_number);
	return chars;


}

int detect_bloc(IplImage *img, int *bloc,int **ptr)
{
	int x,x1,x2,x3,nb_bloc = 0;
	for (x=1 ; x < (img->width-2) ; x++)
	{
		x1 = bloc_value(x-1,img);
		x2 = bloc_value(x,img);
		x3 = bloc_value(x+1,img);

		//printf("x1 : %i ; x2 %i , x3 %i \n",x1,x2,x3);
		if (x2 == 0 && x1 == 1)
		{
			if (nb_bloc != 0)
				bloc = realloc_l(ptr,sizeof(int)*(nb_bloc +1));
			printf("bloc 1 : %i \n",x);
			bloc[nb_bloc] = x;
			nb_bloc++;
		}
		else if (x2 == 0 && x3 == 1)
		{
			
			if (nb_bloc !=0)
				bloc = realloc_l(ptr,sizeof(int)*(nb_bloc + 1));
			bloc[nb_bloc] = x;
			printf("bloc 2 : %i \n",x);
			nb_bloc++;
		}

	}
	

	return nb_bloc;
}



int bloc_value(int x, IplImage *img)
{
	for (int y = 0 ; y <img->height;y++)
	{
		if (get_pixel(img,x,y,0) <50)
			return 1;
	
	}

	return 0;
}

int detect_line(IplImage *img, int *lines_number,int **ptr,int up ,int low,int old_size )
{


	int y,y0,y1,y2,size_lines_number = old_size;

	for (y=1; y< (img->height-1);y++)
	{
		y0 = line_value(img,y-1,up,low);
		y1 = line_value(img,y,up,low);
		y2 = line_value(img,y+ 1,up,low);

		if ( y1==0 && y2==1)
		{
			if (size_lines_number != 0)
				lines_number =realloc_l(ptr,(size_lines_number+1)*sizeof(int));

			lines_number[size_lines_number] = y;
			size_lines_number ++;
		}
		else if (y1==0 && y0==1)
		{
			lines_number =realloc_l(ptr,(size_lines_number+1)*sizeof(int));

			lines_number[size_lines_number] = y;
			size_lines_number ++;

		}
	}


    return size_lines_number;
}


int line_value (IplImage *img,int y,int up, int low)
{
    int x;

    for (x = up ; x <low; x++)
    {
        // if black pixel return 1
        if (get_pixel(img,x,y,0) <50)
        {
            return 1;
        }
    }
    return 0;
}




int detect_char (IplImage *img,int *lines_number,int line_numbers_size,struct rect_char *chars,struct rect_char **ptr,int up,int low,int old_size_c)
{

    // detection space 
    int white = 0;
    int first_char = 0;

    int i,x,x1,x2,x3,mem_x1,mem_x2,c = old_size_c,white_ref;

    for (i=0;i< line_numbers_size;i= i+2)
    {
        mem_x1 = -1;
        mem_x2 = -1;
        white = 0;
        first_char = 0;
        white_ref = 0.18 * (lines_number[i+1]-lines_number[i]);
        if (white_ref >10)
            white_ref = 10;

        for(x=up ; x < low; x++)
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
                    chars[c].x = chars[c-2].x +chars[c - 2].width +2;
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
        if (get_pixel(img,x,i,0) < 50)
        {
            return 1;

        }

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

int color(IplImage *img,int size_c, struct rect_char *chars)
{

    for (int i = 0 ; i < size_c ; i++)
    {
	for (int y = chars[i].y ; y < chars[i].y + chars[i].height ; y++)
	{
		set_pixel(img,y,chars[i].x, 0);
		set_pixel(img,y,chars[i].x + chars[i].width,0);
	}
	
	for(int x = chars[i].x ; x < chars[i].x + chars[i].width; x++)
	{
		set_pixel(img,chars[i].y,x,0);
		set_pixel(img,chars[i].y + chars[i].height,x,0);

	}
    
    }

    return 0;

}
