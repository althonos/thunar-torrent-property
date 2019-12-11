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

#ifndef __TORRENT_PROVIDER_H__
#define __TORRENT_PROVIDER_H__

#include <thunarx/thunarx.h>

G_BEGIN_DECLS;

/* Declare the final type */
#define TORRENT_TYPE_PROVIDER (torrent_provider_get_type())
G_DECLARE_FINAL_TYPE(TorrentProvider, torrent_provider, TORRENT, PROVIDER, GObject);

/* Declare methods */
extern void   torrent_provider_register_type       (ThunarxProviderPlugin *plugin);
extern GType  torrent_provider_get_type            (void);

static void   torrent_provider_class_init          (TorrentProviderClass *klass);
static void   torrent_provider_init                (TorrentProvider *torrent_provider);
static void   torrent_provider_finalize            (GObject *object);
static void   torrent_provider_page_provider_init  (ThunarxPropertyPageProviderIface *iface);
static void   torrent_provider_prefs_provider_init (ThunarxPreferencesProviderIface *iface);
static GList* torrent_provider_get_pages           (ThunarxPropertyPageProvider *property_page_provider,
                                                    GList *files);

G_END_DECLS;

#endif
