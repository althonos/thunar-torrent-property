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

/* This header can be read by both C and C++ compilers */
#ifndef __TORRENT_INFO_HPP__
#define __TORRENT_INFO_HPP__

#include <thunarx/thunarx.h>
#include "../utils/fs.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    char*         name;            // Name of the torrent
    char*         creator;         // Software which created the torrent
    char*         comment;         // Optional comment
    unsigned char infohash[20];    // SHA-1 Infohash
    int           trackerc;        // Number of trackers
    char**        trackerv;        // URL of each tracker
    Filesystem*   files;           // File hierarchy in the torrent
} TorrentInfo;


/* Create a TorrentInfo from a file path */
extern TorrentInfo* torrent_info_new();
extern TorrentInfo* torrent_info_from_thunarx_file_info(ThunarxFileInfo* info);
extern TorrentInfo* torrent_info_from_torrent_file(const char* filename);
extern void torrent_info_delete(TorrentInfo* info);


#ifdef __cplusplus
}
#endif

#endif
