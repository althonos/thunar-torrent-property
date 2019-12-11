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
#ifndef __TORRENT_STATUS_HPP__
#define __TORRENT_STATUS_HPP__

#include <thunarx/thunarx.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int leechers;
    int seeders;
} TorrentStatus;


extern TorrentStatus* torrent_status_new();
extern TorrentStatus* torrent_status_from_thunarx_file_info(ThunarxFileInfo* info);
extern TorrentStatus* torrent_status_from_torrent_file(const char* filename);
extern void torrent_status_delete(TorrentStatus* status);


#ifdef __cplusplus
}
#endif

#endif
