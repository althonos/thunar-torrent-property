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

#include <string.h>
#include <stdlib.h>
#include "fs.h"


extern Filesystem* filesystem_new() {
    Filesystem* fs = (Filesystem*) malloc(sizeof(Filesystem));
    fs->root = entry_new("/");
    return fs;
}

extern Entry* entry_new_with_size(const char* name, size_t size) {
    Entry* entry = (Entry*) malloc(sizeof(Entry));
    entry->child = NULL;
    entry->brother = NULL;
    entry->name = strdup(name);
    entry->size = size;
    return entry;
}

extern Entry* entry_new(const char* name) {
  return entry_new_with_size(name, 0);
}

extern void filesystem_add_file(Filesystem* fs, const char* path, size_t size) {

  Entry* entry = fs->root;
  char* path_copy = strdup(path);

  char* partial_name = strtok(path_copy, "/");
  while (partial_name != NULL) {

    // The entry has no child: we create one
    if (entry->child == NULL) {
      entry->child = entry_new(partial_name);
      entry = entry->child;

    // The child is the entry we want
    } else if (strcmp(entry->child->name, partial_name) == 0) {
      entry = entry->child;

    // The child is not the entry we want: we look at its brothers
    } else {

      // Iterator over the brothers
      Entry* brother = entry->child;

      // Keep browsing until either the iterator is exhausted
      // or we found a brother with the right name
      while ((brother->brother != NULL)
            && (strcmp(brother->brother->name, partial_name) != 0))
        brother = brother->brother;

      // If we exhausted the iterator, we create a new entry
      if (brother->brother == NULL)
          brother->brother = entry_new(partial_name);

      // Update the entry we were looking for.
      entry = brother->brother;
    }

    partial_name = strtok(NULL, "/");
    entry->size += size;
  }

  // The last created entry is the file we wanted.
  //entry->size = size;
  free(path_copy);
}

extern void filesystem_free(Filesystem* fs) {
  entry_free(fs->root);
  free(fs);
}

extern void entry_free(Entry* entry) {

  if (entry->brother != NULL)
    entry_free(entry->brother);
  if (entry->child != NULL)
    entry_free(entry->child);

  free(entry->name);
  free(entry);
}
