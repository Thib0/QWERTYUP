#include <stdlib.h>
#include <gtk/gtk.h>
#include "gtk.h"
#include "image_handle.h"

int main (int argc, char *argv[])
{
  //setWindow(argc, argv);
  int x, y;
  imageFromFile("lol.jpg",&x,&y);
  return 0;
}
