#define _XOPEN_SOURCE 500
#include "stddef.h"
#include "ftw.h"
#include "files.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

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

char* read_file_content(const char* full_path, size_t* out_size) {

    FILE* file = fopen(full_path, "rb");
    if (!file) {
        perror("Произошла ошибка при открытии файла");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = malloc(file_size + 1);
    if (!content) {
        perror("malloc");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("fread");
        free(content);
        fclose(file);
        return NULL;
    }

    content[file_size] = '\0';
    *out_size = file_size;
    fclose(file);
    return content;
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
        char** new_contents = realloc(global_files_list->file_contents, global_files_list->capacity * sizeof(char*));
        size_t* new_sizes = realloc(global_files_list->file_sizes, global_files_list->capacity * sizeof(size_t));

        if (!new_files || !new_contents || !new_sizes) {
            perror("realloc");
            return -1;
        }

        global_files_list->files = new_files;
        global_files_list->file_contents = new_contents;
        global_files_list->file_sizes = new_sizes;
    }   

    global_files_list->files[global_files_list->count] = strdup(file_name);

    size_t content_size;
    char* content = read_file_content(fpath, &content_size);
    if (!content) return -1;

    global_files_list->file_contents[global_files_list->count] = content;
    global_files_list->file_sizes[global_files_list->count] = content_size;
    global_files_list->count++;

    return 0;
}

char** collect_project_files(const char* project_dir, ListFiles* files_list) {
    global_files_list = files_list;
    nftw(project_dir, collect_file, 20, FTW_F);

    return files_list->files;

}
