#include <stdio.h>
#include <gtk/gtk.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "neural_network.h"
#include <time.h>
#include "image_treatment.h"
#include "fileio.h"
#include "image_detection.h"
#include "string.h"
#define ALPHA 0.3
#define BETA 0.9

/* Declaration of vars*/
GtkWidget *window = NULL;
GtkBuilder *builder = NULL;
GError *error = NULL;
gchar *filename = NULL;
GtkImage *srcImgBox = NULL;
GtkEntry *pathTextBox = NULL;
GtkTextView *outputBox = NULL;

void learnAlphabet(neural_network *network);

char *alphabet =
    " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
unsigned count = 0;

G_MODULE_EXPORT void load_image()
{
	const gchar* text = gtk_entry_get_text(pathTextBox);
	gtk_image_set_from_file(srcImgBox, text);
    count = 0;
}

G_MODULE_EXPORT void button1_clicked()
{
	printf("Test 1 \n");
    // traitements d'image
    char *path = (char *)gtk_entry_get_text(pathTextBox);
    char imgPath[7] = "tmp.bmp";

    IplImage *img = !count ? load(path) : load(imgPath);

    if (!img) {
        printf("error loading image\n");
        return;
    }
    /*int mat[9];
    if (count == 1)
         mat[9] = {1, 1, 1, 1, 5, 1, 1, 1, 1};
    else if (count == 2)
         mat[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
    switch (count)
    {
        case 0:
            gray_s(img);
            break;
        case 1:
            //mat = {1, 1, 1, 1, 5, 1, 1, 1, 1};
            matrix(img,mat,13);
            break;
        case 2:
            //mat = {0, -1, 0, -1, 5, -1, 0, -1, 0};
            matrix(img,mat,1);
            break;
        case 3:
            median(img);
            break;
        case 4:
            binarization(img);
            break;
    }

    count++;*/
    img = treatment(img);
    cvSaveImage(imgPath, img, NULL);

}

G_MODULE_EXPORT void button2_clicked()
{
	printf("Test 2 \n");
    // reseau neuron + text etc
    char *imgpath = (char *)gtk_entry_get_text(pathTextBox);
    IplImage *img = load(imgpath);

    if (!img) {
        printf("Error loading image\n");
        return;
    }
    load_image();
    img = treatment(img);
    int nb_char;
    char path[10] = "result.bmp";
    struct rect_char *chars = learning_detection(img, &nb_char);

    cvSaveImage(&path[0], img, NULL);
    neural_network *network = loadNetwork();
    char *str = getString(chars, nb_char, img, network);
    // set str to Box
    GtkTextBuffer *buffer = gtk_text_buffer_new (gtk_text_tag_table_new ());

    gtk_text_view_set_buffer(outputBox, buffer);
    gtk_text_buffer_set_text (buffer, str,strlen(str));

    saveToFile(str, "text");
    freeNetwork(network);
    free(chars);
    free(str);
}

int main(int argc, char *argv[])
{

/*GTK Initialisation */
gtk_init(&argc, &argv);

/* Builder Initialisation */
builder = gtk_builder_new();

/*Giving XML file path */
filename =  g_build_filename ("QCR.glade", NULL);

/*Loading XML file*/
gtk_builder_add_from_file (builder, filename, &error); 
g_free (filename);

/*Error handle*/
    if (error)
    {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
    }

    /*Loading main window*/
    window = GTK_WIDGET(gtk_builder_get_object(builder, "QCR"));
    srcImgBox = GTK_IMAGE(gtk_builder_get_object(builder, "src"));
    pathTextBox = GTK_ENTRY(gtk_builder_get_object(builder, "text"));
    outputBox = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textview"));


    /*Callback connect*/
    gtk_builder_connect_signals (builder, NULL);

    /*Show window*/
    gtk_widget_show_all (window);
    g_object_unref (G_OBJECT (builder));

    /*Run GTK*/
    gtk_main();

    return 0;
}



double *getCharOutput(char c)
{
    double *charArray = malloc(sizeof(double) * 128);
    for (int i = 0; i < c; i++) {
        charArray[i] = 0;
    }
    charArray[c] = 1;
    for (int i = c + 1; i < 128; i++) {
        charArray[i] = 0;
    }
    return charArray;
}

double *spaceImage()
{
    return calloc(20 * 20, sizeof(double));
}


void learnAlphabet(neural_network *network)
{


    int nb_char;
    IplImage *img = load("images/alphabet.png");
    struct rect_char *rect_chars = learning_detection(img, &nb_char);


    if (!img) {
        printf("error loading image lawl\n");
        return;
    }
    double **input = malloc((nb_char + 1)* sizeof(double *));
    input[0] = spaceImage();

    for (int i = 1; i < nb_char + 1; i++) {
        input[i] = imageArray(create_char(img, rect_chars[i-1]));
    }
    double **output = malloc(sizeof(double *) * 128);

    int i = 0;
    for (char *c = alphabet; *c; c++, i++) {
        output[i] = getCharOutput(*c);
    }

    int num_iter = 30000;
    int report_per_iter = 100;
    int dataRows = nb_char + 1;

    for (int i = 0; i < num_iter; i++) {
        int row = i % dataRows;

        runForward(network, input[row]);
        double err = evalError(network, output[row]);

        if (err > 0.1)
        {
            runBackward(network, input[row], output[row], 0);
        }
        if (!(i % report_per_iter))
        {
            err = evalError(network, output[row]);
            printf("iter = %i err = %f char = %c\n", i, err, alphabet[row]);
        }
        
    }
    
}




