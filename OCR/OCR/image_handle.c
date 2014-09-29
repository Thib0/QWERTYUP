#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define string char[]

/*
    File Content :
    Structs/function for image handling
*/
typedef  unsigned char imagePtr;
//typedef  char[] string;

typedef struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}pixel;
// Loads image from url
int imageFromFile(const char file[], int *width, int *heigth) // Thibault style
{
    int n;
    imagePtr *data = stbi_load(file,width,heigth,&n,3);
    struct pixel image[(*width)*(*heigth)];
    if(data == NULL)
    {
        printf("Failure");
        return 0;
    }


    int x, y;
    int i,j;
    printf("%u\n",n);
    int colorBackground = (data[0] + data[1] + data [2])/3;

    for(i = 0; i < (*width)*(*heigth)*3; i+=3)
    {
        image[i/3].red = data[i]*0.3;
        image[i/3].green = data[i+1]*0.59;
        image[i/3].blue = data[i+2]*0.11;

        int moy = 0;
        moy = (data[i] + data[i+1] + data [i+2])/3;

        if (moy - 10 >= colorBackground || moy > 150)
        {
            data[i] = 255;
            data[i+1] = 255;
            data[i+2] = 255;
        }
        else
        {
            data[i] = 0;
            data[i+1] = 0;
            data[i+2] = 0;
        }
        /*data[i] = moy;
        data[i+1] = moy;
        data[i+2] = moy;*/

    }
    for(i = 0; i < (*width)*(*heigth); i++)
    {
        printf("%i - {%i,%i,%i}\n", i, image[i].red,image[i].green,image[i].blue);
    }

    printf("success - %i",(*width)*(*heigth));
    char chemin[] = "test.bmp";
    stbi_write_bmp(&chemin[0], *width, *heigth, 3, data);
    stbi_image_free(data);
    return 1;
}

int imageFromFile2(const char file[], int *width, int *heigth) // Moray style
{
    int n;
    imagePtr *data = stbi_load(file,width,heigth,&n,3);
    struct pixel image[*width][*heigth];
    if(data == NULL)
    {
        printf("Failure");
        return 0;
    }


    int x, y;
    int i,j;
    printf("%u\n",n);

    for(i = 0; i < (*width)*(*heigth)*3; i+=3)
    {
        image[i % *width][i/ *width].red = data[i];
        image[i % *width][i/ *width].green = data[i+1];
        image[i % *width][i/ *width].blue = data[i+2];
    }
    /*for(i = 0; i < (*width)*(*heigth); i++)
    {
        printf("%i - {%i,%i,%i}\n", i, image[i].red,image[i].green,image[i].blue);
    }*/

    printf("success - %i",(*width)*(*heigth));
    char chemin[] = "test.bmp";
    stbi_write_bmp(&chemin[0], *width, *heigth, 3, imageToData(*width, *heigth, image));
    stbi_image_free(data);
    return 1;
}


imagePtr * imageToData(int width, int heigth, struct pixel image[width][heigth])
{
    imagePtr *data = malloc(sizeof(unsigned char)*width*heigth*3);

    int i;
    for(i = 0; i < width*heigth*3; i+=3)
    {
            data[i]     = image[i%width][i/width].red;
            data[i + 1] = image[i%width][i/width].green;
            data[i + 2] = image[i%width][i/width].blue;
    }
    return data;
}

