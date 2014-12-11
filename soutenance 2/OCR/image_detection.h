#ifndef _IMAGE_DETECTION_H_
#define _IMAGE_DETECTION_H_
#include "opencv/cv.h"
#include "opencv/highgui.h"

struct rect_char
{
    int x;
    int y;
    int width;
    int height;
    int bloc;
    // fonction comme un bool, c'est à dire si section est à 1 il faut \n
    int section;
};

struct rect_char * learning_detection(IplImage *img, int *nb_char);
struct rect_char * detect(IplImage *img, int * nb_char);
int detect_bloc(IplImage *img, int *bloc, int **ptr);
int bloc_value(int x1, IplImage *img);
int detect_line(IplImage *img, int *lines_number,int **ptrnt,int up,int low,
        int old_size);
int line_value (IplImage *img,int y,int up,int low);
int color_line(IplImage *img, int y);
int learning_detect_char (IplImage *img,int *lines_number,
        int line_numbers_size,struct rect_char *chars,struct rect_char **ptr,
        int up,int low,int old_size_c);

int detect_sect(struct rect_char *chars, int start, int end, int width_bloc,
        int end_bloc);
int detect_char (IplImage *img,int *line_numbers,int line_numbers_size,
        struct rect_char *chars,struct rect_char **ptr,int up,int low,
        int old_size_c, int cpt_bloc, int old_size_l);
int column_value(int y1, int y2,int x, IplImage *img);
int color_column(IplImage *img,struct rect_char chars);
struct rect_char* realloc_r(struct rect_char ** ptr, size_t taille);
int * realloc_l(int ** ptr, size_t taille);
int color(IplImage *img, int size_c, struct rect_char *chars);
#endif
