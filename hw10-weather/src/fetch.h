#ifndef FETCH_H_INCLUDED
#define FETCH_H_INCLUDED 1

#include <curl/curl.h>
#include <stdlib.h>

typedef struct FETCH_HttpBody
{
  char* data;
  size_t size;
} FETCH_HttpBody;

CURLcode
fetch(char* url, FETCH_HttpBody* http_body);

#endif // FETCH_H_INCLUDED
