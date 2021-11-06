#include <stdlib.h>
#include <assert.h>
#include "config.h"

int main(int argc, char *argv[])
{
    Args args = parse_args(argc, argv);
    Config config = create_config(args);

    int ch;
    int return_code;
    while ((ch = fgetc(config.input)) != EOF)
    {

        if (ch <= 127)
        {
            return_code = fputc(ch, config.output);
            assert(return_code != EOF && "Ошибка записи в выходной файл");
        }
        else if (ch >= 128 && ch <= 255)
        {
            int utf_code = config.encoding_table[ch - 128];

            // Кодируем utf код (0x0420 1000010000) в utf8 (0xd0a0 1101000010100000)
            // Убираем младшие 6 бит, накладываем маску (0xc0 0b11000000)
            return_code = fputc((utf_code >> 6) | 0xC0, config.output);
            assert(return_code != EOF && "Ошибка записи в выходной файл");

            // Берём младшие 6 бит (0x3F 0b00111111), накладываем маску (0x80 0b10000000()
            return_code = fputc((utf_code & 0x3F) | 0x80, config.output);
            assert(return_code != EOF && "Ошибка записи в выходной файл");
        }
    }

    return_code = fclose(config.input);
    assert(return_code == 0 && "Ошибка закрытия входного файла");

    return_code = fclose(config.output);
    assert(return_code == 0 && "Ошибка закрытия выходного файла");

    exit(EXIT_SUCCESS);
}
