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
    return (TorrentStatus*) malloc(sizeof(TorrentStatus));
}


extern TorrentStatus* torrent_status_from_thunarx_file_info(ThunarxFileInfo* info) {
      GFile* gfile = thunarx_file_info_get_location(info);
      TorrentStatus* status = torrent_status_from_torrent_file(g_file_get_path(gfile));
      g_object_unref(gfile);
      return status;
}


extern TorrentStatus* torrent_status_from_torrent_file(const char* filename) {
        g_message("Creating TorrentStatus from torrent file...");

        libtorrent::time_duration timeout = libtorrent::seconds(10);
        std::error_code ec;
        libtorrent::session session;

        /* Make the session show notification alerts */
        libtorrent::settings_pack sp;
        sp.set_int(libtorrent::settings_pack::alert_mask, libtorrent::alert::stats_notification);
        session.apply_settings(sp);

        /* Prepare adding the torrent to the session */
        libtorrent::add_torrent_params params;
        params.ti = boost::make_shared<libtorrent::torrent_info>(std::string(filename));
        params.flags += libtorrent::add_torrent_params::flags_t::flag_paused;

        /* Get the torrent handle */
        libtorrent::torrent_handle handle = session.add_torrent(params);

        if (ec) {
            std::fprintf(stderr, "%s\n", ec.message().c_str());
            return NULL;
        }

        /* Scrape the tracker to get the number of peers */
        std::cout << std::endl << "Scraping trackers...";
        handle.scrape_tracker();
        std::cout << "Request sent." << std::endl;

        libtorrent::scrape_failed_alert* tpf;
        libtorrent::scrape_reply_alert* tpr;
        std::vector<libtorrent::alert*> alerts;

        do {
            sleep(0.1);
            session.pop_alerts(&alerts);
            for (auto* alert: alerts) {
                tpf = libtorrent::alert_cast<libtorrent::scrape_failed_alert>(alert);
                tpr = libtorrent::alert_cast<libtorrent::scrape_reply_alert>(alert);
            }
        } while (!(tpf || tpr));

        TorrentStatus* status = torrent_status_new();
        status->seeders = handle.status().num_complete;
        status->leechers = handle.status().num_incomplete;

        std::cout << "Num Peers      : " << handle.status().num_peers << std::endl;
        std::cout << "Num Complete   : " << handle.status().num_complete << std::endl;
        std::cout << "Nump Incomplete: " << handle.status().num_incomplete << std::endl;

        return status;
}


extern void torrent_status_delete(TorrentStatus* status) {
    free(status);
}
