#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <thunarx/thunarx.h>

#include "torrent-page.h"
#include "wrappers/torrent-info.h"
#include "wrappers/torrent-status.h"

/* Property identifiers */
enum {
  PROP_0,
  PROP_FILE,
};


struct _TorrentPage {
  ThunarxPropertyPage __parent__;
  ThunarxFileInfo     *file;

  /* Variable labels */
  GtkWidget* title;
  GtkWidget* seeders;
  GtkWidget* leechers;
  GtkWidget* trackers;
  GtkWidget* files;
  GtkWidget* refresh;
};


#include "ui/header.c"
#include "ui/trackers.c"
#include "ui/files.c"




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

  /* Create the container */
  GtkWidget* container = gtk_vbox_new(FALSE, 3);

  /* Create the header */
  GtkWidget* header = torrent_page_new_header(page);

  /* Create an homogeneous view container */
  GtkWidget* view_container = gtk_vbox_new(TRUE, 3);

  /* Create the trackers TreeView */
  GtkWidget* trackers = torrent_page_new_trackers_view(page);
  gtk_box_pack_start(GTK_BOX(view_container), trackers, TRUE, TRUE, 3);

  /* Create the files TreeView */
  GtkWidget* files = torrent_page_new_files_view(page);
  gtk_box_pack_start(GTK_BOX(view_container), files, FALSE, TRUE, 3);

  /* Create a non-homogeneous vertical box with the header & view container */
  gtk_box_pack_start(GTK_BOX(container), header, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(container), view_container, TRUE, TRUE, 3);

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
  // FIXME: torrent_info_delete(info);

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
  g_message("Updating info");

  if (info != NULL) {
    g_message("%s", info->name);
    torrent_page_set_title(torrent_page, info->name);
    torrent_page_set_trackers(torrent_page, info->trackerc, info->trackerv);
    torrent_page_set_files(torrent_page, info->filesc, info->filesv, info->sizev);
  }

  //FIXME torrent_page_spawn_update_thread(torrent_page, info);



}
