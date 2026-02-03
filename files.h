typedef struct {
    char** files;
    char** extensions;
    int extensions_count;
    int count;
    int capacity;
} ListFiles;

char** collect_project_files(const char* project_dir, ListFiles* files_list);
