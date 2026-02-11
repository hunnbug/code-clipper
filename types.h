#pragma once
#include "stddef.h"
#include "stdbool.h"

typedef struct {
    char**  extensions;
    char*   project_dir;
    bool    parse_exts;
    int     first_ext_idx;
    int     extensions_count;
} ParsedArgs;

typedef struct {
    char**      files;
    char**      extensions;
    size_t*     file_sizes;
    int         extensions_count;
    int         count;
    int         capacity;
} ListFiles;

