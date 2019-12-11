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

#ifndef __TORRENT_PAGE_H__
#define __TORRENT_PAGE_H__

#include <thunarx/thunarx.h>
#include "wrappers/torrent-info.h"

G_BEGIN_DECLS;

/** Declare the final type
 *
 * We cannot use `G_DECLARE_FINAL_TYPE` as Thunarx does not provide `autoptr`
 * cleanup functions; instead, we implement the equivalent code manually,
 * without `autoptr` cleanup chaining support.
 */
typedef struct _TorrentPage TorrentPage;
typedef struct _TorrentPageClass TorrentPageClass;

#define TORRENT_TYPE_PAGE            (torrent_page_get_type())
#define TORRENT_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TORRENT_TYPE_PAGE, TorrentPage))
#define TORRENT_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TORRENT_TYPE_PAGE, TorrentPageClass))
#define TORRENT_IS_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TORRENT_TYPE_PAGE))
#define TORRENT_IS_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TORRENT_TYPE_PAGE))

struct _TorrentPageClass {
  ThunarxPropertyPageClass parent_class;
};

/* Declare methods */
       void                torrent_page_register_type        (ThunarxProviderPlugin *plugin);
       GType               torrent_page_get_type             (void);

       GtkWidget          *torrent_page_new                  (ThunarxFileInfo *file) G_GNUC_INTERNAL G_GNUC_MALLOC;
       ThunarxFileInfo    *torrent_page_get_file             (TorrentPage *torrent_page) G_GNUC_INTERNAL;
       void                torrent_page_set_file             (TorrentPage *torrent_page,
                                                              ThunarxFileInfo *file) G_GNUC_INTERNAL;
static void                torrent_page_finalize             (GObject *object);
static void                torrent_page_file_changed         (ThunarxFileInfo *file, gpointer user_data);
static void                torrent_page_get_property         (GObject *object,
                                                              guint prop_id,
                                                              GValue *value,
                                                              GParamSpec *pspec);
static void                torrent_page_set_property         (GObject *object,
                                                              guint prop_id,
                                                              const GValue *value,
                                                              GParamSpec *pspec);
static void                torrent_page_update_info          (TorrentPage *torrent_page,
                                                              TorrentInfo* info);

G_END_DECLS;

#endif
