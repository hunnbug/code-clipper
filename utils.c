#include "sys/stat.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "stddef.h"
#include "stdio.h"
#include "limits.h"
#include "stdlib.h"
#include "types.h"

bool is_directory(const char* path) {
    struct stat statbuf;

    if (path == NULL || path[0] == '\0') {
        return false;
    }

    if (stat(path, &statbuf) != 0) {
        return false;
    }

    return S_ISDIR(statbuf.st_mode);
}   

bool is_file_extension(const char* filename) {
    if (filename == NULL) {
        return false;
    }

    size_t len = strlen(filename);
    if (len < 2 || filename[0] != '.') {
        return false;
    }

    for (size_t i = 1; i < len; ++i) {
        if (!isprint((unsigned char)filename[i]) || filename[i] == ' ' || filename[i] == '\\' || filename[i] == '/') {
            return false;
        }
    }

    return true;
}

char* get_absolute_path(const char* path) {
    return realpath(path, NULL);
}

void cleanup_files_list(ListFiles* list) {
    for (int i = 0; i < list->count; ++i) {
        if (list->files[i]) free(list->files[i]);
        if (list->file_paths[i]) free(list->file_paths[i]);
    }

    if (list->files) free(list->files);
    if (list->file_paths) free(list->file_paths); 
    if (list->file_sizes) free(list->file_sizes);

    
    list->files = NULL;
    list->file_paths = NULL;
    list->file_sizes = NULL;
    list->count = 0;
    list->capacity = 0;
    list->extensions_count = 0;


}       

void cleanup_parsed_args(ParsedArgs* args) {
    if (args->project_dir) {
        free(args->project_dir);
        args->project_dir = NULL;
    }

    if (args->extensions) {
        free(args->extensions);
        args->extensions = NULL;
    }

    args->extensions_count = 0;
    args->parse_exts = false;
    args->first_ext_idx = 0;
}

