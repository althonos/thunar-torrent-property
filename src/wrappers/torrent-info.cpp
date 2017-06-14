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
    info->name = strdup(ti.name().c_str());
    info->creator = strdup(ti.creator().c_str());
    info->comment = strdup(ti.comment().c_str());

    /* Put the trackers in an array of char* */
    info->trackerv = (char**) malloc(ti.trackers().size() * sizeof(char*));
    info->trackerc = 0;
    for(const auto& tracker: ti.trackers()) {
        info->trackerv[info->trackerc] = strdup(tracker.url.c_str());
        info->trackerc += 1;
    }

    info->files = filesystem_new();
    for (int i=0; i < ti.files().num_files(); i++) {
        #ifndef NDEBUG
        g_message("%s (%i B)",
                  ti.files().file_path(i).c_str(),
                  ti.files().file_size(i));
        #endif

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
    /* TODO */
    free(info->name);
    free(info->comment);
    free(info->creator);

    for (int i=0; i < info->trackerc; i++)
        free(info->trackerv[i]);
    free(info->trackerv);

    // for (int i=0; i < info->filesc; i++)
    //     free(info->filesv[i]);
    // free(info->filesv);
    // free(info->sizev);

    filesystem_free(info->files);

    free(info);
}
