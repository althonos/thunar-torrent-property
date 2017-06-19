#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <gdk/gdk.h>

#include "../utils/fs.h"
#include "../utils/human-size.h"


static GtkWidget* torrent_page_new_files_view(TorrentPage* page) {

  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkCellRenderer* icon_renderer = gtk_cell_renderer_pixbuf_new();
  GtkWidget* window = gtk_scrolled_window_new(NULL, NULL);
  GtkTreeStore* store = gtk_tree_store_new(3, G_TYPE_ICON, G_TYPE_STRING, G_TYPE_STRING);
  GtkWidget* tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  g_object_unref(G_OBJECT(store));

  GtkTreeViewColumn* column;
  column = gtk_tree_view_column_new_with_attributes("Type", icon_renderer, "gicon", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
  column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
  column = gtk_tree_view_column_new_with_attributes("Size", renderer, "text", 2, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

  page->files = tree_view;
  gtk_container_add(GTK_CONTAINER(window), tree_view);
  return window;
}




static void fill_tree_store(Entry* entry, GtkTreeIter* parent, GtkTreeStore* store) {

  /* Add a new row to the tree store */
  GtkTreeIter iter;
  gtk_tree_store_append(store, &iter, parent);

  /* Get file & folder icons */
  GIcon* entry_icon;
  if (entry->child != NULL) {  // Some child --> this is a directory
    entry_icon = g_icon_new_for_string("folder", NULL);
  } else {
    gchar* entry_type = g_content_type_guess (entry->name, NULL, 0, NULL);
    entry_icon = g_content_type_get_icon(entry_type);
    g_free(entry_type);
  }

  /* Set values of the new row */
  char* hsize = human_size(entry->size);
  gtk_tree_store_set(GTK_TREE_STORE(store), &iter, 0, entry_icon, 1, entry->name, 2, hsize, -1);
  free(hsize);

  /* Add entries for brother and children if any */
  if (entry->brother != NULL)
    fill_tree_store(entry->brother, parent, store);
  if (entry->child != NULL)
    fill_tree_store(entry->child, &iter, store);
}




static void torrent_page_set_files(TorrentPage* page, Filesystem* fs) {

  /* Create a new tree store */
  GtkTreeStore* new_store = gtk_tree_store_new(3, G_TYPE_ICON, G_TYPE_STRING, G_TYPE_STRING);

  /* Remove the old model from the tree view */
  gtk_tree_view_set_model(GTK_TREE_VIEW(page->files), NULL);

  /* Fill the new tree store */
  fill_tree_store(fs->root->child, NULL, new_store);

  /* Create and set a new model from the new tree store */
  gtk_tree_view_set_model(GTK_TREE_VIEW(page->files), GTK_TREE_MODEL(new_store));

  /* Clean */
  g_object_unref(G_OBJECT(new_store));
}
