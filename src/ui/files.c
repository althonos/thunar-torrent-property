#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>

#include "../utils/fs.h"


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




static void fill_tree_store(Entry* entry, GtkTreeIter* parent, GtkTreeStore* store) {

  printf("Adding %s to tree store\n", entry->name);

  GtkTreeIter iter;
  printf("Adding new row\n");
  gtk_tree_store_append(store, &iter, parent);

  printf("Setting values: %s (%i B)\n", entry->name, entry->size);
  gtk_tree_store_set(GTK_TREE_STORE(store), &iter, 0, entry->name, 1, entry->size, -1);

  if (entry->brother != NULL)
    fill_tree_store(entry->brother, parent, store);
  if (entry->child != NULL)
    fill_tree_store(entry->child, &iter, store);
}




static void torrent_page_set_files(TorrentPage* page, Filesystem* fs) {

  //GtkListStore* new_list_store = gtk_list_store_new(3, GTK_TYPE_STRING, GTK_TYPE_STRING, GTK_TYPE_STRING);
  GtkTreeStore* new_store = gtk_tree_store_new(2, GTK_TYPE_STRING, GTK_TYPE_LONG);

  gtk_tree_view_set_model(GTK_TREE_VIEW(page->files), NULL);

  fill_tree_store(fs->root->child, NULL, new_store);

  gtk_tree_view_set_model(GTK_TREE_VIEW(page->files), GTK_TREE_MODEL(new_store));
  g_object_unref(G_OBJECT(new_store));
  filesystem_free(fs);
}
