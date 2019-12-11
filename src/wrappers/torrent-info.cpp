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

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include <thunarx/thunarx.h>
#include <boost/shared_ptr.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/announce_entry.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/settings_pack.hpp>
#include <libtorrent/add_torrent_params.hpp>

#include "torrent-info.h"
#include "../utils/fs.h"

#include <gtk/gtk.h>


extern TorrentInfo* torrent_info_new() {
    return (TorrentInfo*) g_malloc(sizeof(TorrentInfo));
}


extern TorrentInfo* torrent_info_from_torrent_file(const char* filename) {
    /* Create a new TorrentInfo */
    TorrentInfo* info = torrent_info_new();

    /* Get libtorrent::torrent_info of torrent file */
    libtorrent::torrent_info ti = libtorrent::torrent_info(std::string(filename));

    /* Update fields on the TorrentInfo struct */
    info->name = strdup(ti.name().c_str());
    info->creator = strdup(ti.creator().c_str());
    info->comment = strdup(ti.comment().c_str());

    /* Extract info hash as hexadecimal string */
    libtorrent::sha1_hash infohash = ti.info_hash();
    for (size_t i = 0; i < 20; ++i) {
        info->infohash[i] = infohash[i];
    }

    /* Put the trackers in an array of char* */
    info->trackerv = (char**) g_malloc(ti.trackers().size() * sizeof(char*));
    info->trackerc = 0;
    for(const auto& tracker: ti.trackers()) {
        info->trackerv[info->trackerc] = strdup(tracker.url.c_str());
        info->trackerc += 1;
    }

    info->files = filesystem_new();
    for (int i=0; i < ti.files().num_files(); i++) {
        g_debug(
          "File: %s (%i B)",
          ti.files().file_path(i).c_str(),
          ti.files().file_size(i)
        );
        filesystem_add_file(
            info->files,
            ti.files().file_path(i).c_str(),
            ti.files().file_size(i));
    }

    /* Return the updated struct */
    return info;
}


extern TorrentInfo* torrent_info_from_thunarx_file_info(ThunarxFileInfo* info) {
      /* Get a GFile pointer from the ThunarxFileInfo* */
      GFile* gfile = thunarx_file_info_get_location(info);
      /* Create a new TorrentInfo from the torrent file */
      TorrentInfo* torrent_info = torrent_info_from_torrent_file(g_file_get_path(gfile));
      /* Free the GFile pointer */
      g_object_unref(gfile);
      return torrent_info;
}


extern void torrent_info_delete(TorrentInfo* info) {
    free(info->name);
    free(info->comment);
    free(info->creator);

    for (int i=0; i < info->trackerc; i++)
        free(info->trackerv[i]);
    free(info->trackerv);

    filesystem_free(info->files);

    free(info);
}
