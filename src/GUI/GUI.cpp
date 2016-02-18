#include <stdio.h>
#include <iostream>

#include "../Disasm/Disasm.hpp"

#include <gtk/gtk.h>


int main(int argc, char *argv[]) {


    GtkWidget* win;


    gtk_init(&argc, &argv);

    /* Create Window */
    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /* Set size */
    gtk_window_set_default_size(GTK_WINDOW(win), 500, 500);
    gtk_container_set_border_width(GTK_CONTAINER(win), 5);
    /* Stuff */

    GtkWidget* vbox;
    GtkWidget* b0, *b1;

    vbox = gtk_vbox_new(1, 1);
    gtk_container_add(GTK_CONTAINER(win), vbox);

    b0 = gtk_button_new_with_label("Disassembly");
    b1 = gtk_button_new_with_label("Run");

    gtk_box_pack_start(GTK_BOX(vbox), b0, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(vbox), b1, 1, 1, 0);


    /* Stuff end */
    gtk_widget_show_all(win);
    gtk_main();


    return 0;
}
