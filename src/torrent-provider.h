#ifndef __TORRENT_PROVIDER_H__
#define __TORRENT_PROVIDER_H__

#include <thunarx/thunarx.h>

G_BEGIN_DECLS;


#define TORRENT_TYPE_PROVIDER (torrent_provider_get_type())
G_DECLARE_FINAL_TYPE(
    TorrentProvider,
    torrent_provider,
    TORRENT, PROVIDER,
    GObject);

extern void torrent_provider_register_type(ThunarxProviderPlugin *plugin);
extern GType torrent_provider_get_type();

static void torrent_provider_class_init (TorrentProviderClass *klass);
static void torrent_provider_init (TorrentProvider *torrent_provider);
static void torrent_provider_finalize (GObject *object);
static void torrent_provider_page_provider_init (ThunarxPropertyPageProviderIface *iface);
static void torrent_provider_prefs_provider_init (ThunarxPreferencesProviderIface *iface);
static GList* torrent_provider_get_pages (ThunarxPropertyPageProvider *property_page_provider, GList *files);


G_END_DECLS;

#endif
