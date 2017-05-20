#include <gio/gio.h>


static GtkWidget* torrent_page_new_files_view(TorrentPage* page) {

  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkCellRenderer* icon_renderer = gtk_cell_renderer_pixbuf_new();
  GtkWidget* window = gtk_scrolled_window_new(NULL, NULL);
  //GtkListStore* store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
  GtkListStore* store = gtk_list_store_new(2, G_TYPE_STRING, GTK_TYPE_LONG);
  GtkWidget* tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  g_object_unref(G_OBJECT(store));


  GtkTreeViewColumn* column;
  // column = gtk_tree_view_column_new_with_attributes("Icon", icon_renderer, "icon_name", 0, NULL);
  // gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
  column = gtk_tree_view_column_new_with_attributes("Path", renderer, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
  column = gtk_tree_view_column_new_with_attributes("Size (B)", renderer, "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

  page->files = tree_view;
  gtk_container_add(GTK_CONTAINER(window), tree_view);
  return window;
}


static void torrent_page_set_files(TorrentPage* page, long int size, char** files, long int* sizes) {

  //GtkListStore* new_list_store = gtk_list_store_new(3, GTK_TYPE_STRING, GTK_TYPE_STRING, GTK_TYPE_STRING);
  GtkListStore* new_list_store = gtk_list_store_new(2, GTK_TYPE_STRING, GTK_TYPE_LONG);
  gtk_tree_view_set_model(GTK_TREE_VIEW(page->files), NULL);

  GtkTreeIter iter;
  for (int i=0; i < size; i++) {
    g_message(files[i]);
    gtk_list_store_append(GTK_LIST_STORE(new_list_store), &iter);
    gtk_list_store_set(GTK_LIST_STORE(new_list_store), &iter, 0, files[i], 1, sizes[i], -1);
  }

  gtk_tree_view_set_model(GTK_TREE_VIEW(page->files), GTK_TREE_MODEL(new_list_store));
  g_object_unref(G_OBJECT(new_list_store));

}






//
//
// const char *file_name = "test.html";
//   gboolean is_certain = FALSE;
//
//   char *content_type = g_content_type_guess (file_name, NULL, 0, &is_certain);
//
//   if (content_type != NULL)
//     {
//       char *mime_type = g_content_type_get_mime_type (content_type);
//
//       g_print ("Content type for file '%s': %s (certain: %s)\n"
//                "MIME type for content type: %s\n",
//                file_name,
//                content_type,
//                is_certain ? "yes" : "no",
//                mime_type);
//
//       g_free (mime_type);
//     }
//
//   g_free (content_type);
//
//   return EXIT_SUCCESS;
