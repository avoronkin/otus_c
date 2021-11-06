#ifndef CONFIG_HEADER_FILE
#define CONFIG_HEADER_FILE

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct Args
{
    char *input_file_name;
    char *output_file_name;
    char *encoding_table_name;
    bool help;
} Args;

typedef struct Config
{
    FILE *input;
    FILE *output;
    uint16_t encoding_table[128];
} Config;

Args parse_args(int argc, char *argv[]);

Config create_config(Args args);

void print_help();

#endif
