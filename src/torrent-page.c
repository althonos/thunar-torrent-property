#include <gtk/gtk.h>
#include <thunarx/thunarx.h>

#include "torrent-page.h"
#include "torrent-info.h"


/* Property identifiers */
enum {
  PROP_0,
  PROP_FILE,
};


struct _TorrentPage {
  ThunarxPropertyPage __parent__;
  ThunarxFileInfo     *file;
  TorrentInfo         *info;

  GtkWidget           *container_vbox;

  /* Header */
  GtkWidget           *header_hbox;
  GtkWidget           *header_image;
  GtkWidget           *header_title;
  GtkWidget           *header_sep;

  /* Trackers list */
  GtkTreeView         *trackers_tree_view;
};

/* implements the torrent_page_get_type() and torrent_page_register_type() functions */
THUNARX_DEFINE_TYPE(TorrentPage, torrent_page, THUNARX_TYPE_PROPERTY_PAGE);

static void torrent_page_class_init (TorrentPageClass *klass) {
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = torrent_page_finalize;
  gobject_class->get_property = torrent_page_get_property;
  gobject_class->set_property = torrent_page_set_property;

  /**
  * TorrentPage:file:
  *
  * The ThunarxFileInfo displayed by this TorrentPage.
  **/
  g_object_class_install_property (
    gobject_class,
    PROP_FILE,
    g_param_spec_object (
      "file", "file", "file",
      THUNARX_TYPE_FILE_INFO,
      G_PARAM_READWRITE
    )
  );
}


static void torrent_page_init (TorrentPage *page) {

  /****************\
  * Main container *
  \****************/
  page->container_vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (page->container_vbox), 5);

  // /* Header */
  page->header_hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (page->container_vbox), page->header_hbox, FALSE, TRUE, 5);

  page->header_image = gtk_image_new ();
  gtk_image_set_from_icon_name (GTK_IMAGE (page->header_image), "application-torrent", GTK_ICON_SIZE_DIALOG);
  gtk_misc_set_padding (GTK_MISC (page->header_image), 13, 0);
  gtk_misc_set_alignment (GTK_MISC (page->header_image), 0.0, 0.50);
  gtk_box_pack_start (GTK_BOX (page->header_hbox), page->header_image, FALSE, FALSE, 0);

  page->header_title = gtk_label_new ("<b>Torrent</b>");
  gtk_label_set_use_markup (GTK_LABEL(page->header_title), TRUE);
  gtk_misc_set_alignment (GTK_MISC (page->header_title), 0.0f, 0.50f);
  gtk_box_pack_start (GTK_BOX (page->header_hbox), page->header_title, FALSE, TRUE, 5);

  /* Horizontal separator */
  page->header_sep = gtk_hseparator_new();
  gtk_box_pack_start (GTK_BOX (page->container_vbox), page->header_sep, FALSE, TRUE, 5);


  /**************\
  * Tracker list *
  \**************/
  //trackers_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  GtkListStore* trackers_list_store = gtk_list_store_new (1, G_TYPE_STRING);
  /* Create a view */
  page->trackers_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (trackers_list_store));
  /* The view now holds a reference.  We can get rid of our own reference */
  g_object_unref (G_OBJECT (trackers_list_store));

  /* Create a cell render and arbitrarily make it red for demonstration
   * purposes */
  GtkCellRenderer* renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "foreground", "black", NULL);

   /* Create a column, associating the "text" attribute of the
    * cell_renderer to the first column of the model */
   GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes (
     "Tracker URL", renderer, "text", 0, NULL
   );

   /* Add the column to the view. */
   gtk_tree_view_append_column (GTK_TREE_VIEW (page->trackers_tree_view), column);

   /* Add the tree view to the main container */
   gtk_box_pack_start (GTK_BOX (page->container_vbox), page->trackers_tree_view, TRUE, TRUE, 5);


  /* Add to the dialog */
  gtk_container_add (GTK_CONTAINER (page), page->container_vbox);
  gtk_container_set_border_width (GTK_CONTAINER (page), 5);
  gtk_widget_show_all (page->container_vbox);

}







static void torrent_page_finalize (GObject *object) {
  /* Check the object is a TorrentPage */
  TorrentPage *torrent_page = TORRENT_PAGE(object);

  /* disconnect from the file */
  torrent_page_set_file (torrent_page, NULL);

  /* free some memory */
  //delete_torrent_info(torrent_page->info);

  /* Free some memory */
  // if (!G_STR_EMPTY (torrent_page->share_name))
  //   g_free (torrent_page->share_name);
  // if (!G_STR_EMPTY (torrent_page->share_comment))
  //   g_free (torrent_page->share_comment);

  /* Call the destructor of the parent class */
  (*G_OBJECT_CLASS (torrent_page_parent_class)->finalize) (object);
}





static void torrent_page_get_property (
  GObject *object, guint prop_id, GValue *value, GParamSpec *pspec
) {
  TorrentPage *torrent_page = TORRENT_PAGE(object);

  switch (prop_id) {
    case PROP_FILE:
      g_value_set_object (value, torrent_page_get_file (torrent_page));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}







static void torrent_page_set_property (
  GObject *object, guint prop_id, const GValue *value,
  GParamSpec *pspec
) {
  TorrentPage *torrent_page = TORRENT_PAGE(object);

  switch (prop_id) {
    case PROP_FILE:
      torrent_page_set_file (torrent_page, g_value_get_object(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}






GtkWidget* torrent_page_new (ThunarxFileInfo *file) {
  TorrentPage *page = g_object_new (TORRENT_TYPE_PAGE, "file", file, NULL);
  thunarx_property_page_set_label (THUNARX_PROPERTY_PAGE (page), "Torrent");

  TorrentInfo *info = torrent_info_from_file_info(file);

  torrent_page_update_info(page, info);
  return GTK_WIDGET (page);
}






ThunarxFileInfo* torrent_page_get_file (TorrentPage *torrent_page) {
  g_return_val_if_fail (TORRENT_IS_PAGE (torrent_page), NULL);
  return torrent_page->file;
}





void torrent_page_set_file (TorrentPage *torrent_page, ThunarxFileInfo *file) {
  g_return_if_fail (TORRENT_IS_PAGE (torrent_page));
  g_return_if_fail (file == NULL || THUNARX_IS_FILE_INFO (file));

  /* Check if we already use this file */
  if (G_UNLIKELY (torrent_page->file == file))
    return;

  /* Disconnect from the previous file (if any) */
  if (G_LIKELY (torrent_page->file != NULL)) {
    g_signal_handlers_disconnect_by_func (G_OBJECT (torrent_page->file), torrent_page_file_changed, torrent_page);
    g_object_unref (G_OBJECT (torrent_page->file));
  }

  /* Assign the value */
  torrent_page->file = file;

  /* Connect to the new file (if any) */
  if (G_LIKELY (file != NULL)) {
    /* Take a reference on the info file */
    g_object_ref (G_OBJECT (torrent_page->file));
    torrent_page_file_changed (file, torrent_page);
    g_signal_connect (
      G_OBJECT (file), "changed", G_CALLBACK (torrent_page_file_changed),
      torrent_page
    );
  }
}



/* File changed */
static void torrent_page_file_changed (
  ThunarxFileInfo *file, gpointer user_data
) {
}



static void torrent_page_update_info(TorrentPage *torrent_page, TorrentInfo *info) {

  g_message("Updating TorrentPage from TorrentInfo");
  g_message(info->name);
  gtk_label_set_text(GTK_LABEL(torrent_page->header_title), info->name);

  GtkListStore* trackers_list_store = GTK_LIST_STORE(gtk_tree_view_get_model(torrent_page->trackers_tree_view));
  GtkTreeIter iter;

  gtk_list_store_clear (trackers_list_store);

  for (int i=0; i < info->trackerc; i++) {
    g_message(info->trackerv[i]);
    gtk_list_store_append(trackers_list_store, &iter);
    gtk_list_store_set(trackers_list_store, &iter, 0, info->trackerv[i], -1);
  }

  g_message("Successfully updated TorrentPage from TorrentInfo");
}
