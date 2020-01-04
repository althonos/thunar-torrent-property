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

#include <unistd.h>
#include <thunarx/thunarx.h>
#include <boost/shared_ptr.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/time.hpp>
#include <libtorrent/announce_entry.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/settings_pack.hpp>
#include <libtorrent/add_torrent_params.hpp>

#include "torrent-status.h"

#include <iostream> // FIXME: DEBUG


extern TorrentStatus* torrent_status_new() {
    return (TorrentStatus*) g_malloc(sizeof(TorrentStatus));
}


extern TorrentStatus* torrent_status_from_thunarx_file_info(ThunarxFileInfo* info) {
    GFile* gfile = thunarx_file_info_get_location(info);
    TorrentStatus* status = torrent_status_from_torrent_file(g_file_get_path(gfile));
    g_object_unref(gfile);
    return status;
}


extern TorrentStatus* torrent_status_from_torrent_file(const char* filename) {
    g_debug("Creating TorrentStatus from torrent file...");

    /* Use a short timeout to avoid waiting too long */
    libtorrent::time_duration timeout = libtorrent::seconds(5);
    std::error_code ec;
    libtorrent::session session;

    /* Make the session show notification alerts */
    libtorrent::settings_pack sp;
    sp.set_int(libtorrent::settings_pack::alert_mask, libtorrent::alert::stats_notification);
    sp.set_int(libtorrent::settings_pack::connections_limit, 0);
    session.apply_settings(sp);

    /* Prepare adding the torrent to the session */
    libtorrent::add_torrent_params params;
    params.ti = std::make_shared<libtorrent::torrent_info>(std::string(filename));
    params.flags = libtorrent::torrent_flags::paused;

    /* Get the torrent handle */
    libtorrent::torrent_handle handle = session.add_torrent(params);
    if (G_UNLIKELY(ec)) {
        g_error("%s", ec.message().c_str());
        return NULL;
    }

    /* Scrape the tracker to get the number of peers */
    g_debug("Scraping trackers.");
    handle.scrape_tracker();
    g_debug("Request sent.");

    /* Wait until scraping is finished (render UI while waiting) */
    libtorrent::scrape_failed_alert* tpf;
    libtorrent::scrape_reply_alert* tpr;
    std::vector<libtorrent::alert*> alerts;
    do {
        while(gtk_events_pending()) gtk_main_iteration();
        usleep(200000);
        session.pop_alerts(&alerts);
        for (auto* alert: alerts) {
            tpf = libtorrent::alert_cast<libtorrent::scrape_failed_alert>(alert);
            tpr = libtorrent::alert_cast<libtorrent::scrape_reply_alert>(alert);
        }
    } while (!(tpf || tpr));

    /* Extract the seeders/leechers from the torrent handle  */
    TorrentStatus* status = torrent_status_new();
    if (tpr) {
        status->seeders = handle.status().num_complete;
        status->leechers = handle.status().num_incomplete;
        g_debug("Success: %i Leechers, %i Seeders", status->leechers, status->seeders);
    } else {
        status->seeders = -1;
        status->leechers = -1;
        g_debug("Failure: %s", tpf->message().c_str());
    }

    return status;
}


extern void torrent_status_delete(TorrentStatus* status) {
    free(status);
}
