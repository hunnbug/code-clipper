#define _XOPEN_SOURCE 500
#include "stddef.h"
#include "ftw.h"
#include "files.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

static ListFiles* global_files_list = NULL;

bool has_extension(const char* file_name) {
    if (global_files_list->extensions_count == 0) return true;

    const char* ext = strrchr(file_name, '.');
    if (!ext) return false;

    for (int i = 0; i < global_files_list->extensions_count; ++i) {
        if (strcmp(ext, global_files_list->extensions[i]) == 0) return true;
    }
    return false;
}

int collect_file(const char* fpath, const struct stat* sb,
                 int typeflag, struct FTW* ftwbuf) {

    if (typeflag != FTW_F) return 0;

    const char* file_name = strrchr(fpath, '/'); 
    if (file_name == NULL) file_name = fpath;
    else file_name++;

    if (!has_extension(file_name)) return false;

    if (global_files_list->count >= global_files_list->capacity) {
        
        global_files_list->capacity *= 2;
        char** new_files = realloc(global_files_list->files, global_files_list->capacity * sizeof(char*));

        global_files_list->files = new_files;
    }   

    global_files_list->files[global_files_list->count] = strdup(file_name);

    global_files_list->count++;

    return 0;
}

char** collect_project_files(const char* project_dir, ListFiles* files_list) {
    global_files_list = files_list;
    nftw(project_dir, collect_file, 20, FTW_F);

    return files_list->files;

}
