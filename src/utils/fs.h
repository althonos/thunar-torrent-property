#ifndef __FS_H__
#define __FS_H__

typedef struct _Filesystem Filesystem;
typedef struct _Entry entry;

struct _Entry {
  Entry*,
  Entry*,
  char*,
  size_t
};

struct _Filesystem {
  _Entry*
};

extern Filesystem* filesystem_new();
extern Entry* entry_new_with_size(const char*, size_t);
extern Entry* entry_new(const char*);
extern void filesystem_add_file(Filesystem*, const char*, size_t);

#endif
