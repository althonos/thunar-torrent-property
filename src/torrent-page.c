#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <thunarx/thunarx.h>

#include "torrent-page.h"
#include "torrent-info.h"

#include "ui/header.c"
#include "ui/trackers.c"
#include "ui/files.c"



/* Property identifiers */
enum {
  PROP_0,
  PROP_FILE,
};


struct _TorrentPage {
  ThunarxPropertyPage __parent__;
  ThunarxFileInfo     *file;
  TorrentInfo         *info;

  // /* Renderers */
  // GtkCellRenderer     *renderer_trackers;
  // GtkCellRenderer     *renderer_files;
  // GtkCellRenderer     *renderer_file_icons;
  //
  // /* Page container */
  // GtkWidget           *container_vbox;
  //
  // /* Header */
  // GtkWidget           *header_hbox;
  // GtkWidget           *header_image;
  // GtkWidget           *header_title;
  // GtkWidget           *header_sep;
  //
  // /* Trackers list */
  // GtkWidget           *trackers_scrolled_window;
  // GtkWidget           *trackers_tree_view;
  //
  // /* Files list */
  // GtkWidget           *files_scrolled_window;
  // GtkWidget           *files_tree_view;
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


  //
  //
  // /****************\
  // *    Renderers   *
  // \****************/
  // page->renderer_trackers = gtk_cell_renderer_text_new();
  // page->renderer_files = gtk_cell_renderer_text_new();
  // page->renderer_file_icons = gtk_cell_renderer_pixbuf_new();
  // // g_object_set(G_OBJECT(renderer), "foreground", "black", NULL);
  // // g_object_set(G_OBJECT(renderer), "background", "white", NULL);
  //
  //
  //
  // /**************\
  // * Tracker list *
  // \**************/
  // page->trackers_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  // trackers_list_store = gtk_list_store_new(1, G_TYPE_STRING);
  // page->trackers_tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL (trackers_list_store));
  // g_object_unref(G_OBJECT(trackers_list_store));
  //
  // column = gtk_tree_view_column_new_with_attributes ("Tracker URL", page->renderer_trackers, "text", 0, NULL);
  // gtk_tree_view_append_column (GTK_TREE_VIEW (page->trackers_tree_view), column);
  //
  // gtk_container_add(GTK_CONTAINER(page->trackers_scrolled_window), page->trackers_tree_view);
  // gtk_box_pack_start (GTK_BOX (page->container_vbox), page->trackers_scrolled_window, TRUE, TRUE, 5);
  //
  //
  // /**************\
  // *  Files list  *
  // \**************/
  // page->files_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  // files_tree_store = gtk_tree_store_new(3, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_INT);
  // page->files_tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(files_tree_store));
  // g_object_unref(G_OBJECT(trackers_list_store));
  //
  // column = gtk_tree_view_column_new_with_attributes ("Icon", page->renderer_file_icons, "icon-name", 0, NULL);
  // gtk_tree_view_append_column (GTK_TREE_VIEW (page->files_tree_view), column);
  // column = gtk_tree_view_column_new_with_attributes ("Path", page->renderer_files, "text", 1, NULL);
  // gtk_tree_view_append_column (GTK_TREE_VIEW (page->files_tree_view), column);
  // column = gtk_tree_view_column_new_with_attributes ("Size", page->renderer_files, "text", 2, NULL);
  // gtk_tree_view_append_column (GTK_TREE_VIEW (page->files_tree_view), column);
  //
  // gtk_container_add(GTK_CONTAINER(page->files_scrolled_window), page->files_tree_view);
  // gtk_box_pack_start (GTK_BOX (page->container_vbox), page->files_scrolled_window, TRUE, TRUE, 5);
  //
  //
  // /* Add to the dialog */
  // gtk_container_add (GTK_CONTAINER (page), page->container_vbox);
  // gtk_container_set_border_width (GTK_CONTAINER (page), 5);
  // gtk_widget_show_all (page->container_vbox);


  /* Create the container */
  GtkWidget* container = gtk_vbox_new(FALSE, 3);

  /* Create the header and add the header to the page */
  GtkWidget* header = torrent_page_new_header(page);
  gtk_box_pack_start(GTK_BOX(container), header, FALSE, TRUE, 3);

  /* Create the trackers TreeView */
  //GtkWidget* trackers = torrent_page_new_trackers_view(page);
  //gtk_box_pack_start(GTK_BOX(container), trackers, FALSE, TRUE, 3);

  /* Create the files TreeView */
  //GtkWidget* files = torrent_page_new_files_view(page);
  //gtk_box_pack_start(GTK_BOX(container), files, FALSE, TRUE, 3);

  /* Add the container to the page */
  gtk_container_add (GTK_CONTAINER(page), container);
  gtk_container_set_border_width (GTK_CONTAINER(page), 5);
  gtk_widget_show_all(container);

}











static void torrent_page_finalize (GObject *object) {
  /* Check the object is a TorrentPage */
  TorrentPage *torrent_page = TORRENT_PAGE(object);

  /* disconnect from the file */
  torrent_page_set_file (torrent_page, NULL);

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

  TorrentInfo *info = torrent_info_from_thunarx_file_info(file);
  torrent_page_update_info(page, info);
  torrent_info_delete(info);

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

  // g_message("Updating TorrentPage from TorrentInfo");
  // g_message(info->name);
  // gtk_label_set_text(GTK_LABEL(torrent_page->header_title), info->name);
  //
  // /* Update the trackers list */
  // GtkListStore *new_list_store;
  // GtkTreeIter iter;
  // gtk_tree_view_set_model(torrent_page->trackers_tree_view, NULL); /* unrefs the old model */
  // new_list_store = gtk_list_store_new (1, GTK_TYPE_STRING);
  // for (int i=0; i < info->trackerc; i++) {
  //   g_message(info->trackerv[i]);
  //   gtk_list_store_append(GTK_LIST_STORE(new_list_store), &iter);
  //   gtk_list_store_set(GTK_LIST_STORE(new_list_store), &iter, 0, info->trackerv[i], -1);
  // }
  // gtk_tree_view_set_model(torrent_page->trackers_tree_view, new_list_store);
  // g_object_unref (new_list_store);
  //
  //
  // g_message("Successfully updated TorrentPage from TorrentInfo");
}
