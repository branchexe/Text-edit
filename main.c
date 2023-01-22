#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define MAX_LINE_LENGTH 256

static GtkTextBuffer* buffer;

static void open_file(GtkWidget* widget, gpointer data) {
    GtkWidget* dialog = gtk_file_chooser_dialog_new("Open File",
                                                    GTK_WINDOW(data),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                                    NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }

        char line[MAX_LINE_LENGTH];
        GtkTextIter iter;
        gtk_text_buffer_get_start_iter(buffer, &iter);
        while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
            gtk_text_buffer_insert(buffer, &iter, line, -1);
        }
        fclose(file);
    }
    gtk_widget_destroy(dialog);
}

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Text Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget* menubar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    GtkWidget* fileMenu = gtk_menu_new();
    GtkWidget* fileMi = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi),fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);

    GtkWidget* openMi = gtk_menu_item_new_with_label("Open");
    g_signal_connect(openMi, "activate", G_CALLBACK(open_file), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);

    GtkWidget* saveMi = gtk_menu_item_new_with_label("Save");
    // Add code here to handle the "Save" menu item
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveMi);

    GtkWidget* quitMi = gtk_menu_item_new_with_label("Quit");
    g_signal_connect(quitMi, "activate", G_CALLBACK(gtk_main_quit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);

    GtkWidget* textView = gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    gtk_box_pack_start(GTK_BOX(vbox), textView, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

