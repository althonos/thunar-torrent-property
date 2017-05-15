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


// FIXME: DEBUG
#include <gtk/gtk.h>





extern TorrentInfo* torrent_info_new() {
    return (TorrentInfo*) malloc(sizeof(TorrentInfo));
}


extern TorrentInfo* torrent_info_from_torrent_file(const char* filename) {

    /* Create a new TorrentInfo */
    TorrentInfo* info = torrent_info_new();

    /* Get libtorrent::torrent_info of torrent file */
    libtorrent::torrent_info ti = libtorrent::torrent_info(std::string(filename));

    /* Update fields on the TorrentInfo struct */
    info->name = (char*) malloc(ti.name().size() * sizeof(char));
    strcpy(info->name, ti.name().c_str());
    info->creator = (char*) malloc(ti.creator().size() * sizeof(char));
    strcpy(info->creator, ti.creator().c_str());
    info->comment = (char*) malloc(ti.comment().size() * sizeof(char));
    strcpy(info->comment, ti.comment().c_str());

    /* Put the trackers in an array of char* */
    info->trackerv = (char**) malloc(ti.trackers().size() * sizeof(char*));
    info->trackerc = 0;
    for(const auto& tracker: ti.trackers()) {
        info->trackerv[info->trackerc] = (char*) malloc(tracker.url.size() * sizeof(char*));
        strcpy(info->trackerv[info->trackerc], tracker.url.c_str());
        info->trackerc += 1;
    }

    /* Return the update struct */
    return info;
}








// extern TorrentInfo* create_torrent_info(const char* filename) {
//
//     g_message("Creating TorrentInfo from torrent file...");
//
//     TorrentInfo* info = (TorrentInfo*) malloc(sizeof(TorrentInfo));
//
//     std::error_code ec;
//     libtorrent::session session;
//
//     /* Make the session show notification alerts */
//     libtorrent::settings_pack sp;
//     sp.set_int(libtorrent::settings_pack::alert_mask, libtorrent::alert::stats_notification);
//     session.apply_settings(sp);
//
//
//     /* Prepare adding the torrent to the session */
//     libtorrent::add_torrent_params params;
//     params.ti = boost::make_shared<libtorrent::torrent_info>(std::string(filename));
//     params.flags += libtorrent::add_torrent_params::flags_t::flag_paused;
//
//
//     /* Get the torrent handle */
//     libtorrent::torrent_handle handle = session.add_torrent(params);
//
//     if (ec) {
//         std::fprintf(stderr, "%s\n", ec.message().c_str());
//         return NULL;
//     }
//
//
//     /* Scrape the tracker to get the number of peers */
//     handle.scrape_tracker();
//
//     libtorrent::scrape_failed_alert* tpf;
//     libtorrent::scrape_reply_alert* tpr;
//     std::vector<libtorrent::alert*> alerts;
//
//     do {
//         sleep(0.1);
//         session.pop_alerts(&alerts);
//         for (auto* alert: alerts) {
//             tpf = libtorrent::alert_cast<libtorrent::scrape_failed_alert>(alert);
//             tpr = libtorrent::alert_cast<libtorrent::scrape_reply_alert>(alert);
//         }
//     } while (!(tpf || tpr)); // QUESTION: maybe add a timeout ?
//
//
//     g_message(handle.torrent_file()->name().c_str());
//
//     /* Copy the name, creator and comment of torrent_handle to the TorrentInfo */
//     info->name = (char*) malloc(handle.torrent_file()->name().size() * sizeof(char));
//     strcpy(info->name, handle.torrent_file()->name().c_str());
//     info->creator = (char*) malloc(handle.torrent_file()->creator().size() * sizeof(char));
//     strcpy(info->creator, handle.torrent_file()->creator().c_str());
//     info->comment = (char*) malloc(handle.torrent_file()->comment().size() * sizeof(char));
//     strcpy(info->comment, handle.torrent_file()->comment().c_str());
//
//     /* Get the number of Seeders and Leechers */
//     info->num_complete = handle.status().num_complete;
//     info->num_incomplete = handle.status().num_incomplete;
//
//     /* Put the trackers in an array of char* */
//     info->trackerv = (char**) malloc(handle.torrent_file()->trackers().size() * sizeof(char*));
//     info->trackerc = 0;
//     for(const auto& tracker: handle.torrent_file()->trackers()) {
//         info->trackerv[info->trackerc] = (char*) malloc(tracker.url.size() * sizeof(char*));
//         strcpy(info->trackerv[info->trackerc], tracker.url.c_str());
//         info->trackerc += 1;
//     }
//
//     g_message("Successfully created TorrentInfo");
//
//     return info;
//
// }





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
    // TODO //
    free(info->name);
    free(info->comment);
    free(info->creator);

    for (int i=0; i < info->trackerc; i++)
        free(info->trackerv[i]);
    free(info->trackerv);
}
