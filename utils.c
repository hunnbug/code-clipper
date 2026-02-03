#include "sys/stat.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "stddef.h"
#include "stdio.h"
#include "unistd.h"
#include "limits.h"
#include "stdlib.h"

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

char* get_current_path() {
    return getcwd(NULL, 0); 
}

char* get_absolute_path(const char* path) {
    return realpath(path, NULL);
}
