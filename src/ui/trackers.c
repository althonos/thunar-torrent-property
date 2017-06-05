


static GtkWidget* torrent_page_new_trackers_view(TorrentPage* page) {


  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkWidget* window = gtk_scrolled_window_new(NULL, NULL);
  GtkListStore* store = gtk_list_store_new(1, G_TYPE_STRING);
  GtkWidget* tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes("Tracker URL", renderer, "text", 0, NULL);

  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
  gtk_container_add(GTK_CONTAINER(window), tree_view);
  g_object_unref(G_OBJECT(store));

  page->trackers = tree_view;

  return window;
}



static void torrent_page_set_trackers(TorrentPage* page, int size, char** trackers) {

  GtkListStore* new_list_store = gtk_list_store_new (1, GTK_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(page->trackers), NULL); /* unrefs the old model */

  GtkTreeIter iter;
  for (int i=0; i < size; i++) {
    #ifndef NDEBUG
      g_message(trackers[i]);
    #endif
    gtk_list_store_append(GTK_LIST_STORE(new_list_store), &iter);
    gtk_list_store_set(GTK_LIST_STORE(new_list_store), &iter, 0, trackers[i], -1);
  }

  gtk_tree_view_set_model(GTK_TREE_VIEW(page->trackers), GTK_TREE_MODEL(new_list_store));
  g_object_unref(G_OBJECT(new_list_store));
}
