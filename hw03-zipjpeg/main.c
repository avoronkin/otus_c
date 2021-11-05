#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

unsigned char SIGNATURE[4] = {0x50, 0x4b, 0x03, 0x04};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Не указан путь к файлу\n");
        fprintf(stderr, "Пример: %s zipjpeg.jpg\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *zipjpeg_path = argv[1];

    FILE *file_ptr = fopen(zipjpeg_path, "r");
    if (file_ptr == NULL)
    {
        fprintf(stderr, "Не удалось открыть файл \"%s\"\n", zipjpeg_path);
        exit(EXIT_FAILURE);
    }

    int header_offset = 0;
    int ch;
    while ((ch = fgetc(file_ptr)) != EOF)
    {
        if (ch != 0x50)
        {
            continue;
        }

        if ((header_offset = ftell(file_ptr) - 1) == -1L)
        {
            fprintf(stderr, "Ошибка при получении позиции файла\n");
            exit(EXIT_FAILURE);
        }

        unsigned char signature[4];
        if (fseek(file_ptr, header_offset, SEEK_SET) != 0)
        {
            fprintf(stderr, "Чтение signature: Не удалось переместить указатель в файле");
            exit(EXIT_FAILURE);
        }
        if (fread(signature, 4, 1, file_ptr) != 1)
        {
            fprintf(stderr, "Чтение signature: Не удалось прочитать файл");
            exit(EXIT_FAILURE);
        }
        if (memcmp(signature, SIGNATURE, sizeof(SIGNATURE)) != 0)
        {
            continue;
        }

        uint16_t file_name_length = 0;
        if (fseek(file_ptr, header_offset + 26, SEEK_SET) != 0)
        {
            fprintf(stderr, "Чтение file_name_length: Не удалось переместить указатель в файле");
            exit(EXIT_FAILURE);
        }
        if (fread(&file_name_length, sizeof(file_name_length), 1, file_ptr) != 1)
        {
            fprintf(stderr, "Чтение file_name_length: Не удалось прочитать файл");
            exit(EXIT_FAILURE);
        }

        char file_name[file_name_length];
        if (fseek(file_ptr, header_offset + 30, SEEK_SET) != 0)
        {
            fprintf(stderr, "Чтение file_name: Не удалось переместить указатель в файле\n");
            exit(EXIT_FAILURE);
        }
        if (fread(file_name, file_name_length, 1, file_ptr) != 1)
        {
            fprintf(stderr, "Чтение file_name: Не удалось прочитать файл\n");
            exit(EXIT_FAILURE);
        }
        file_name[file_name_length] = '\0';

        printf("%s\n", file_name);
    }

    if (fclose(file_ptr) != 0)
    {
        fprintf(stderr, "Не удалось закрыть файл \"%s\"\n", zipjpeg_path);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
