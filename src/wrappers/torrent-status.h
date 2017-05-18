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
