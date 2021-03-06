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

#ifndef __FS_H__
#define __FS_H__


#ifdef __cplusplus
extern "C" {
#endif


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

#ifdef __cplusplus
}
#endif

#endif
