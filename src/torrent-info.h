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


    /*  The number of peers that have downloaded the torrent
     *  completely.
     */
    int num_complete;

    /*  The number of peers that are still not finished
     *  downloading the torrent.
     */
    int num_incomplete;

    /* The number of trackers in trackerv */
    int trackerc;

    /* A char* array containing the URL of the trackers */
    char** trackerv;


} TorrentInfo;



/* Create a TorrentInfo from a file path */
extern TorrentInfo* create_torrent_info(const char* filename);
extern TorrentInfo* torrent_info_from_file_info(ThunarxFileInfo* info);
extern void delete_torrent_info(TorrentInfo* info);

#ifdef __cplusplus
}
#endif

#endif
