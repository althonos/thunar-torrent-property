/* This header can be read by both C and C++ compilers */
#ifndef __TORRENT_INFO_HPP__
#define __TORRENT_INFO_HPP__

#include <thunarx/thunarx.h>


#ifdef __cplusplus
extern "C" {
#endif



typedef struct {

    /* Name of the torrent */
    char* name;

    /* Tool the torrent was created with */
    char* creator;

    /* An optional comment */
    char* comment;

    /* The number of trackers in trackerv */
    int trackerc;

    /* A char* array containing the URL of the trackers */
    char** trackerv;

    /* The number of files in filesv */
    int filesc;

    /* A char* array containing the path of the files in the torrent */
    char** filesv;

    /* A uint array containing the size of the files in the torrent */
    uint* sizev;


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
