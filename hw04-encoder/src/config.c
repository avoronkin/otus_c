#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "encoding_tables.h"
#include "config.h"

Args parse_args(int argc, char *argv[])
{
    Args args = {
        .input_file_name = NULL,
        .output_file_name = NULL,
        .encoding_table_name = NULL,
        .help = false,
    };

    for (int i = 1; i < argc; i++)
    {
        char *key = argv[i];
        char *value = argv[i + 1];

        if (strcmp(key, "-h") == 0)
        {
            args.help = true;
            i++;
        }
        else if (strcmp(key, "-i") == 0)
        {
            args.input_file_name = value;
            i++;
        }
        else if (strcmp(key, "-o") == 0)
        {
            args.output_file_name = value;
            i++;
        }
        else if (strcmp(key, "-e") == 0)
        {
            args.encoding_table_name = value;
            i++;
        }
    }

    return args;
}

Config create_config(Args args)
{
    if (args.help)
    {
        print_help();
        exit(EXIT_FAILURE);
    }

    if (args.encoding_table_name == NULL)
    {
        fprintf(stderr, "Ошибка: не указана кодировка\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    Config config = {
        .input = stdin,
        .output = stdout,
        .encoding_table = {0}
    };

    if (strcmp(args.encoding_table_name, "koi8") == 0)
    {
        memcpy(&config.encoding_table, &koi8_to_utf8, sizeof(koi8_to_utf8));
    }
    else if (strcmp(args.encoding_table_name, "cp1251") == 0)
    {
        memcpy(&config.encoding_table, &cp1251_to_utf8, sizeof(cp1251_to_utf8));
    }
    else if (strcmp(args.encoding_table_name, "iso8859") == 0)
    {
        memcpy(&config.encoding_table, &iso8859_to_utf8, sizeof(iso8859_to_utf8));
    }
    else
    {
        fprintf(stderr, "Ошибка: неизвестная кодировка%s\n", args.encoding_table_name);
        print_help();
        exit(EXIT_FAILURE);
    }

    if (args.input_file_name != NULL)
    {
        config.input = fopen(args.input_file_name, "r");
        if (config.input == NULL)
        {
            fprintf(stderr, "Ошибка: не удалось открыть входной файл %s\n", args.input_file_name);
            exit(EXIT_FAILURE);
        }
    }

    if (args.output_file_name != NULL)
    {
        config.output = fopen(args.output_file_name, "w");
        if (config.output == NULL)
        {
            fprintf(stderr, "Ошибка: не удалось открыть выходной файл %s\n", args.output_file_name);
            exit(EXIT_FAILURE);
        }
    }

    return config;
}

void print_help()
{
    fprintf(stderr, "Преобразует указанные файлы из кодировки(cp1251|koi8|iso8859) в utf8\n"
                    "Использование: encoder -e koi8 [-i input.txt] [-o output.txt]\n"
                    "Параметры:\n"
                    "  -e кодировка оригинального текста\n"
                    "  -i входной файл\n"
                    "  -o выходной файл\n"
                    "  -h справка\n"
                    "Пример: encoder -e cp1251 -i input.txt -o output.txt\n"
                    "Пример: encoder -e cp1251 -i input.txt\n"
                    "Пример: encoder -e cp1251\n");
}
