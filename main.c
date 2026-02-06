#include "utils.h"
#include "files.h"
#include "errors.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        PRINT_ARGUMENTS_ERROR;
        return 1;
    }

    bool first_is_dir = is_directory(argv[1]);
    bool first_is_ext = is_file_extension(argv[1]);
    
    ParsedArgs parsed_args = {
        .extensions = NULL,
        .project_dir = NULL,
        .parse_exts = false,
        .first_ext_idx = 0,
        .extensions_count = 0
    };

    for (int i = 2; i < argc; ++i) {
        if (is_directory(argv[i])) {
            errno = 0;
            PRINT_USAGE_ERROR(argv[i]);
            return 1;
        }
    }

    if (first_is_ext) {
        
        parsed_args.project_dir = getcwd(NULL, 0); 

        parsed_args.first_ext_idx = 1;
        parsed_args.parse_exts = true;
        parsed_args.extensions_count = argc - 1;

    } else if (first_is_dir) {

        if (argc > 2) {
            parsed_args.first_ext_idx = 2;
            parsed_args.parse_exts = true;
            parsed_args.extensions_count = argc - 2;
        } else {
            parsed_args.parse_exts = false;
        }
        
        if (get_absolute_path(argv[1]) != NULL) {
            parsed_args.project_dir = get_absolute_path(argv[1]);
        } else {
            PRINT_USAGE_ERROR("Ошибка при получении пути проекта");
            return 1;
        }

    } else {
        PRINT_USAGE_ERROR(argv[1]);
        return 1; 
    }

    printf("В качестве директории проекта используется %s.\n", parsed_args.project_dir);

    if (parsed_args.parse_exts) {
        parsed_args.extensions = malloc(parsed_args.extensions_count * sizeof(char*));
        int extensions_idx = 0;
        for (int i = parsed_args.first_ext_idx; i < argc; ++i) {
            if (!is_file_extension(argv[i])) {
                PRINT_USAGE_ERROR(argv[i]);
                cleanup_parsed_args(&parsed_args);
                return 1;
            } else {
                parsed_args.extensions[extensions_idx++] = argv[i];
            }
        }
    }

    if (parsed_args.extensions_count > 0 && parsed_args.extensions != NULL) {
        printf("Ищем расширения: ");
        for (int i = 0; i < parsed_args.extensions_count; ++i) {
            printf("%s ", parsed_args.extensions[i]);
        }
        printf("\n");
    }

    ListFiles list_files = {
        .files = malloc(512 * sizeof(char*)),
        .file_contents = malloc(512 * sizeof(char*)),
        .file_paths = malloc(512 * sizeof(char*)),
        .file_sizes = malloc(512 * sizeof(size_t)),
        .extensions = parsed_args.extensions,
        .extensions_count = parsed_args.extensions_count,
        .count = 0,
        .capacity = 512 
    };

    char** files = collect_project_files(parsed_args.project_dir, &list_files);

    printf("\nНайдены файлы:\n");
    for (int i = 0; i < list_files.count; ++i) {
        printf(" | %s %d байт\n", files[i], list_files.file_sizes[i]);
    }

    printf("\nВсего прочитано и скопировано: %d файлов\n", list_files.count);

    cleanup_parsed_args(&parsed_args);
    cleanup_files_list(&list_files);

    return 0;
}
