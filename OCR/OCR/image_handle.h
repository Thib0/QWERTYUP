#define STB_IMAGE_IMPLEMENTATION
#define string char[]

struct pixel;
int imageFromFile(const char file[], int *width, int *heigth);
int imageFromFile2(const char file[], int *width, int *heigth);
