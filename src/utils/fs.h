#ifndef __FS_H__
#define __FS_H__

typedef struct _Filesystem Filesystem;
typedef struct _Entry Entry;

struct _Filesystem {
    Entry* root;
};

struct _Entry {
    Entry* child;
    Entry* brother;
    char* name;
    size_t size;
};

extern Entry* entry_new_with_size(const char*, size_t);
extern Entry* entry_new(const char*);
extern void entry_free(Entry*);
extern Filesystem* filesystem_new();
extern void filesystem_add_file(Filesystem*, const char*, size_t);
extern void filesystem_free(Filesystem*);

#endif
