#include <stdlib.h>
#include <gtk/gtk.h>
#include "gtk.h"
#include "image_handle.h"


int main (int argc, char *argv[])
{
  //setWindow(argc, argv);
  int x, y;
  imageFromFile("test_scan.png", &x, &y);
  return 0;
}
