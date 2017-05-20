/* This header can be read by both C and C++ compilers */
#ifndef __TORRENT_INFO_HPP__
#define __TORRENT_INFO_HPP__

#include <thunarx/thunarx.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    char*  name;            // Name of the torrent
    char*  creator;         // Software which created the torrent
    char*  comment;         // Optional comment
    int    trackerc;        // Number of trackers
    char** trackerv;        // URL of each tracker
    int    filesc;          // Number of files
    char** filesv;          // URL of each file
    long* sizev;            // Size of each file
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
