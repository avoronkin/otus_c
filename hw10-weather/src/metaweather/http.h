#ifndef FETCH_H_INCLUDED
#define FETCH_H_INCLUDED

#include <curl/curl.h>
#include <stdlib.h>

typedef struct Http_ResBody
{
  char* data;
  size_t size;
} Http_ResBody;

CURLcode
http_get(char* url, Http_ResBody* http_res_body);

#endif // FETCH_H_INCLUDED
