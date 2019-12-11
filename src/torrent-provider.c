/*
 * Copyright (C) 2017-2019 Martin Larralde <martin.larralde@ens-paris-saclay.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <thunarx/thunarx.h>

#include "torrent-provider.h"
#include "torrent-page.h"


struct _TorrentProvider
{
  GObject       __parent__;
};

THUNARX_DEFINE_TYPE_WITH_CODE(
  TorrentProvider,
  torrent_provider,
  G_TYPE_OBJECT,
  THUNARX_IMPLEMENT_INTERFACE (
    THUNARX_TYPE_PROPERTY_PAGE_PROVIDER,
    torrent_provider_page_provider_init)
  THUNARX_IMPLEMENT_INTERFACE (
    THUNARX_TYPE_PREFERENCES_PROVIDER,
    torrent_provider_prefs_provider_init));




static void torrent_provider_class_init (TorrentProviderClass *klass) {
  GObjectClass *gobject_class;
  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = torrent_provider_finalize;
}



static void torrent_provider_init (TorrentProvider *torrent_provider) {

}



static void torrent_provider_finalize (GObject *object) {
  (*G_OBJECT_CLASS (torrent_provider_parent_class)->finalize) (object);
}



static void torrent_provider_page_provider_init(ThunarxPropertyPageProviderIface *iface) {
  iface->get_pages = torrent_provider_get_pages;
}



static void torrent_provider_prefs_provider_init(ThunarxPreferencesProviderIface *iface) {

}



static GList* torrent_provider_get_pages (ThunarxPropertyPageProvider *property_page_provider,
                                          GList *files) {
  if (g_list_length (files) != 1) {
    #ifndef NDEBUG
      g_warning("MULTIPLE FILES SELECTED");
    #endif
    return NULL;
  } else if (!thunarx_file_info_has_mime_type(files->data, "application/x-bittorrent")) {
    #ifndef NDEBUG
      g_warning("NOT A TORRENT FILE");
    #endif
    return NULL;
  }

  #ifndef NDEBUG
    g_message("TORRENT FILE OK !");
  #endif

  GtkWidget* torrent_page = torrent_page_new(files->data);

  #ifndef NDEBUG
    g_message("Appending TorrentPage to GList");
  #endif
  return g_list_append(NULL, (gpointer) torrent_page);//torrent_page_new(files->data));
}
