#include <stdio.h>
#include <gtk/gtk.h>

/* Declaration of vars*/
GtkWidget *window = NULL;
GtkBuilder *builder = NULL; 
GError *error = NULL;
gchar *filename = NULL;
GtkImage *srcImgBox = NULL;
GtkEntry *pathTextBox = NULL;

G_MODULE_EXPORT void load_image()
{
	const gchar* text = gtk_entry_get_text(pathTextBox);
	gtk_image_set_from_file(srcImgBox, text);
}

G_MODULE_EXPORT void button1_clicked()
{
	printf("Test 1 \n");
}

G_MODULE_EXPORT void button2_clicked()
{
	printf("Test 2 \n");
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



/*Callback connect*/
gtk_builder_connect_signals (builder, NULL);

/*Show window*/
gtk_widget_show_all (window); 
g_object_unref (G_OBJECT (builder));

/*Run GTK*/
gtk_main();

return 0;
}