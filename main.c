#include "utils.h"
#include "errors.h"
#include "stdbool.h"
#include "stdio.h"

typedef struct {
    char**  extensions;
    char*   project_dir;
    bool    parse_exts;
    int     first_ext_idx;
    int     num_of_extensions;
} ParsedArgs;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        PRINT_ARGUMENTS_ERROR;
        return 1;
    }

    bool first_is_dir = is_directory(argv[1]);
    bool first_is_ext = is_file_extension(argv[1]);
    
    ParsedArgs parsed_args;

    if (first_is_ext) {
        
        parsed_args.num_of_extensions = argc - 1;

        if (get_current_path() != NULL) {
            parsed_args.project_dir = get_current_path();
        } else {
            perror("Произошла непредвиденная ошибка при получении пути к проекту, приносим свои извинения.\n");
            return 1;
        }

        parsed_args.first_ext_idx = 1;
        parsed_args.parse_exts = true;

    } else if (first_is_dir) {

        if (argc > 2) {
            parsed_args.num_of_extensions = argc - 2;
            parsed_args.first_ext_idx = 2;
        } else {
            parsed_args.parse_exts = false;
        }
        
        if (get_absolute_path(argv[1]) != NULL) {
            parsed_args.project_dir = get_absolute_path(argv[1]);
        } else {
            perror("Произошла непредвиденная ошибка при получении пути к проекту, приносим свои извинения.\n"); // сделать макрос с errno
            return 1;
        }

    } else {
        PRINT_USAGE_ERROR(argv[1]);
        return 1; 
    }

    printf("В качестве директории проекта используется %s.\n", parsed_args.project_dir);

    return 0;
}
