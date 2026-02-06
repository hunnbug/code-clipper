#include "stdbool.h"
#include "types.h"

bool is_directory(const char*);
bool is_file_extension(const char*);
char* get_absolute_path(const char*);
void cleanup_parsed_args(ParsedArgs*);
void cleanup_files_list(ListFiles*);
