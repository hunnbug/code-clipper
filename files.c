#define _XOPEN_SOURCE 500
#define _GNU_SOURCE
#include "stddef.h"
#include "ftw.h"
#include "sys/stat.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "types.h"

static ListFiles* global_files_list = NULL;
static FILE* temp = NULL;

bool has_extension(const char* file_name) {

    const char* ext = strrchr(file_name, '.');
    if (!ext) return false;

    if (global_files_list->extensions_count == 0) return true;

    for (int i = 0; i < global_files_list->extensions_count; ++i) {
        if (strcmp(ext, global_files_list->extensions[i]) == 0) return true;
    }
    return false;
}

size_t read_file_content(const char* full_path) {

    FILE* file = fopen(full_path, "rb");
    if (!file) {
        perror("Произошла ошибка при открытии файла");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < 0) {
        fclose(file);
        return -1;
    }

    char* content = malloc(file_size + 1);
    if (!content) {
        perror("malloc");
        free(content);
        fclose(file);
        return -1;
    }

    size_t bytes_read = fread(content, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("fread");
        fclose(file);
        return -1;
    }

    content[file_size] = '\0';

    fprintf(temp, "Файл: %s\n", full_path);

    if (fputs(content, temp) == EOF) fprintf(stderr, "Произошла ошибка при открытии файла %s", full_path);

    fprintf(temp, "\n\n");

    fclose(file);
    free(content);
    return file_size;
}


int collect_file(const char* fpath, const struct stat* sb,
                 int typeflag, struct FTW* ftwbuf) {

    if (typeflag == FTW_SL || typeflag == FTW_SLN) return 0;

    if (typeflag == FTW_D || typeflag == FTW_DP) {
        const char* dir_name = strrchr(fpath, '/');
        if (dir_name == NULL) dir_name = fpath;
        else dir_name++;
        
        if (dir_name[0] == '.' && strcmp(dir_name, ".") != 0 && strcmp(dir_name, "..") != 0) {
            return FTW_SKIP_SUBTREE; 
        }
        return 0;
    }

    if (!S_ISREG(sb->st_mode)) return 0;

    const char* file_name = strrchr(fpath, '/'); 
    if (file_name == NULL) file_name = fpath;
    else file_name++;

    if (file_name[0] == '.') {
        return 0;
    }

    if (!has_extension(file_name)) return 0;

    if (global_files_list->count >= global_files_list->capacity) {
        
        global_files_list->capacity *= 2;
        char** new_files = realloc(global_files_list->files, global_files_list->capacity * sizeof(char*));
        size_t* new_sizes = realloc(global_files_list->file_sizes, global_files_list->capacity * sizeof(size_t));

        if (!new_files || !new_sizes/* || !new_file_paths */) {
            perror("пиздец!");
            free(new_files);
            free(new_sizes);
            return -1;
        }

        global_files_list->files = new_files;
        global_files_list->file_sizes = new_sizes;

    }   

    char* file_name_copy = strdup(file_name);
    if (!file_name_copy) {
        return 0;
    }

    size_t content_size = read_file_content(fpath);

    global_files_list->files[global_files_list->count] = file_name_copy;
    global_files_list->file_sizes[global_files_list->count] = content_size;
    global_files_list->count++;

    return 0;
}

char** collect_project_files(const char* project_dir, ListFiles* files_list) {
    global_files_list = files_list;
    int flags = FTW_PHYS | FTW_ACTIONRETVAL;

    temp = fopen("/tmp/cclipper_content", "w");
    if (!temp) {
        perror("Не удалось создать временный файл\n");
        return NULL;
    }

    nftw(project_dir, collect_file, 20, flags);

    fclose(temp);

    system("cat /tmp/cclipper_content | xclip -selection clipboard");
    
    remove("/tmp/cclipper_content");

    return files_list->files;

}
