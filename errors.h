#include "stdio.h"
#include "errno.h"

#define PRINT_USAGE_ERROR(arg) \
    switch (errno) {\
        case EACCES: \
            fprintf(stderr, "Ошибка. Нет разрешения для использования пути до проекта\n"); \
            break; \
        case EBADF: \
            fprintf(stderr, "Ошибка. Неподдерживаемый формат дескриптора пути до проекта\n"); \
            break; \
        case EFAULT: \
            fprintf(stderr, "Ошибка. Невозможно получить доступ к директории проекта\n"); \
            break; \
        case ELOOP: \
            fprintf(stderr, "Ошибка. Количество системных ссылок в пути превысило системные ограничения\n"); \
            break; \
        case ENAMETOOLONG: \
            fprintf(stderr, "Ошибка. Слишком длинный путь до проекта\n"); \
            break; \
        case ENOENT: \
            fprintf(stderr, "Ошибка. Путь до проекта не существует\n"); \
            break; \
        case ENOMEM: \
            fprintf(stderr, "Ошибка. Out of kernel memory\n"); \
            break; \
        case ENOTDIR: \
            fprintf(stderr, "Ошибка. Путь к проекту не является директорией\n"); \
            break; \
        case EOVERFLOW: \
            fprintf(stderr, "Ошибка. Невозможно получить доступ к директории проекта на текущей разрядности системы\n"); \
            break; \
        case 0: \
            fprintf(stderr, "Ошибка. Неправильный аргумент: %s.\n\n", arg); \
            fprintf(stderr, "Правильные варианты использования: \n\ncclipper путь/к/директории/проекта .расширения .имён .файлов\n\ncclipper .расширения .имён .файлов. В таком случае директорией проекта будет считаться директория, из которой была запущена утилита cclipper\n\ncclipper В таком случае будет произведена работа с файлами со всеми расширениями в директории, откуда запускается cclipper\n"); break; \
    };

#define PRINT_ARGUMENTS_ERROR fprintf(stderr, "Ошибка. Недостаточно аргументов\nПравильное использование: cclipper путь/к/директории/проекта .расширения .имён .файлов\nЛибо cclipper .расширения .имён .файлов. В таком случае директорией проекта будет считаться директория, из которой была запущена утилита cclipper\n")
