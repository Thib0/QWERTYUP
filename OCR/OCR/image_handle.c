#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#define string char[]

/*
    File Content :
    Structs/function for image handling
*/
typedef  unsigned char imagePtr;
//typedef  char[] string;

struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
// Loads image from url
int imageFromFile(const char file[], int *width, int *heigth)
{
    int n;
    imagePtr *data = stbi_load(file,width,heigth,&n,3);
    struct pixel image[(*width)*(*heigth)];
    printf("%i",data);
    if(data == NULL)
    {
        printf("Failure");
        return 0;
    }


    int x, y;
    int i,j;
    printf("%u\n",n);

    for(i = 0; i < (*width)*(*heigth); i+=3)
    {
        image[i].red = data[i];
        image[i].green = data[i+1];
        image[i].blue = data[i+2];
        printf("{%i,%i,%i}\n",image[i].red,image[i].green,image[i].blue);
    }

    printf("success");
    stbi_image_free(data);
    return 1;
}
