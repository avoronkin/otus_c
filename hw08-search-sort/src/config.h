#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED 1

#include <stdio.h>
#include <stdlib.h>

typedef struct config
{
  FILE* input;
} cfg;

cfg
cf_create(int argc, char** argv);

#endif // CONFIG_H_INCLUDED
