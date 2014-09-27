#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.c"
#define string char[]

/*
    File Content :
    Structs/function for image handling
*/
typedef  unsigned char imagePtr;
//typedef  char[] string;

struct pixel
{
    char red;
    char green;
    char blue;
};
// Loads image from url
int imageFromFile()
{
    printf("begin");
    int width,height,n;
    imagePtr *data = stbi_load("image.bmp",&width,&height,&n,0);
    struct pixel image[width][height];
    if(data == NULL)
    {
        printf("Failure");
        return 0;
    }


    int x, y;
    int i,j;
    for(x = 0, i = 0; x < sizeof(data) + x*y*n; x += y*n*sizeof(*data), i++)
    {
        for(y = x,j = 0; y < x; y += n*sizeof(*data),j++)
        {
            int r,g,b;
            r = data[x+y];
            g = data[x+y+8];
            b = data[x+y+16];
            image[i][j].red = r;
            image[i][j].green = g;
            image[i][j].blue = b;
            printf("{%i %i %i}", image[i][j].red,image[i][j].green,image[i][j].blue);
        }
        printf("\n");
    }
    printf("success");
    return 1;
}
