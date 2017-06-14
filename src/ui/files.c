#include <string.h>
#include <stdlib.h>
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






typedef struct _Entry Entry;
struct _Entry{
  Entry* child;
  Entry* brother;
  char* name;
  size_t size;
};

static Entry* new_entry(char* name) {
  Entry* entry = (Entry*) malloc(sizeof(Entry));
  entry->child = NULL;
  entry->brother = NULL;
  entry->name = strdup(name);
  entry->size = 0;
  return entry;
}

static void add_entry(Entry* root, char* path, size_t size) {

  Entry* entry = root;
  char* path_copy = strdup(path);

  char* partial_name = strtok(path_copy, "/");
  while (partial_name != NULL) {
    printf ("%s\n",partial_name);

    if (entry->child == NULL) {
      entry->child = new_entry(partial_name);
      entry = entry->child;

    } else if (strcmp(entry->child->name, partial_name) == 0){
      entry = entry->child;

    } else {

      Entry* brother = entry->child;

      while ((brother->brother != NULL) && (strcmp(brother->brother->name, partial_name) != 0)) {
        brother = brother->brother;
      }

      if (brother->brother == NULL)
        brother->brother = new_entry(partial_name);

      entry = brother->brother;
    }

    char* aux = partial_name;
    partial_name = strtok (NULL, "/");
  }

  free(path_copy);
  entry->size = size;
}

static void free_entry(Entry* entry) {
  free(entry->name);

  if ((entry->child) != NULL)
    free(entry->child);
  if ((entry->brother) != NULL)
    free(entry->brother);

  free(entry);
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






static void torrent_page_set_files(TorrentPage* page, long int size, char** files, long int* sizes) {

  //GtkListStore* new_list_store = gtk_list_store_new(3, GTK_TYPE_STRING, GTK_TYPE_STRING, GTK_TYPE_STRING);
  GtkTreeStore* new_store = gtk_tree_store_new(2, GTK_TYPE_STRING, GTK_TYPE_LONG);

  gtk_tree_view_set_model(GTK_TREE_VIEW(page->files), NULL);

  Entry* root = new_entry("/");
  for (int i=0; i < size; i++) {
    #ifndef NDEBUG
      g_message(files[i]);
    #endif
    add_entry(root, files[i], sizes[i]);
  }

  fill_tree_store(root->child, NULL, new_store);

  gtk_tree_view_set_model(GTK_TREE_VIEW(page->files), GTK_TREE_MODEL(new_store));
  g_object_unref(G_OBJECT(new_store));
  free(root);
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
