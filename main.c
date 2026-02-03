#include "utils.h"
#include "errors.h"
#include "stdbool.h"
#include "stdio.h"
#include "limits.h"

typedef struct {
    char* project_dir;
    bool parse_exts;
    int first_ext_idx;
} ParsedArgs;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        PRINT_ARGUMENTS_ERROR;
        return 1;
    }

    bool first_is_ext = is_file_extension(argv[1]);
    bool first_is_dir = is_directory(argv[1]);

    int num_of_extensions;
    char work_directory;

    ParsedArgs parsed_args;

    if (first_is_ext) {

        num_of_extensions = argc - 1;

        // for (int i = 1; i < argc; ++i) {
        //     if (!is_file_extension(argv[i])) {
        //         PRINT_USAGE_ERROR(argv[i]);
        //         return 1;
        //     }
        // }
        
        char cwd[PATH_MAX];
        get_current_path(cwd);
       
        if (cwd != NULL) {
            parsed_args.project_dir = cwd;
        } else {
            perror("Произошла непредвиденная ошибка, просим прощения");
            return 2;
        }

        parsed_args.first_ext_idx = 1;

        printf("В качестве директории проекта используется %s.\n", cwd);

    } else if (first_is_dir) {

        if (argc > 2) {
            num_of_extensions = argc - 2;
            parsed_args.first_ext_idx = 2;
        } else {
            parsed_args.parse_exts = false;
        }

        parsed_args.project_dir = argv[1];

        printf("В качестве директории проекта используется %s.", argv[1]); // переделать на абсолютный путь


    } else {
        
        PRINT_USAGE_ERROR(argv[1]);
        return 1;
                                                                                   
    }

    return 0;
}
