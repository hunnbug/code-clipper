#include "stddef.h"
typedef struct {
    char**      files;
    char**      extensions;
    char**      file_contents;
    size_t*     file_sizes;
    int         extensions_count;
    int         count;
    int         capacity;
} ListFiles;

char** collect_project_files(const char* project_dir, ListFiles* files_list);
