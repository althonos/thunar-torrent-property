#ifndef __TORRENT_PAGE_H__
#define __TORRENT_PAGE_H__

#include <thunarx/thunarx.h>
#include "wrappers/torrent-info.h"

G_BEGIN_DECLS;

/* Declare the final type */
#define TORRENT_TYPE_PAGE (torrent_page_get_type())
G_DECLARE_FINAL_TYPE(TorrentPage, torrent_page, TORRENT, PAGE, ThunarxPropertyPage);

/* Declare methods */
extern GtkWidget *torrent_page_new(ThunarxFileInfo *file) G_GNUC_INTERNAL G_GNUC_MALLOC;
extern ThunarxFileInfo *torrent_page_get_file(TorrentPage *torrent_page) G_GNUC_INTERNAL;
extern void torrent_page_set_file(TorrentPage *torrent_page, ThunarxFileInfo *file) G_GNUC_INTERNAL;
extern void torrent_page_register_type(ThunarxProviderPlugin *plugin);
extern GType torrent_page_get_type();


static void torrent_page_finalize(GObject *object);
static void torrent_page_file_changed(ThunarxFileInfo *file, gpointer user_data);
static void torrent_page_get_property(GObject *object, guint prop_id,
                                      GValue *value, GParamSpec *pspec);
static void torrent_page_set_property(GObject *object, guint prop_id,
                                      const GValue *value, GParamSpec *pspec);
static void torrent_page_update_info(TorrentPage *torrent_page, TorrentInfo* info);

G_END_DECLS;

#endif /* !__torrent_page_H__ */
