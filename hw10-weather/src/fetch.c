#include "fetch.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

static size_t
WriteHttpBodyCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
  size_t realsize = size * nmemb;
  FETCH_HttpBody* http_body = (FETCH_HttpBody*)userp;

  char* ptr = realloc(http_body->data, http_body->size + realsize + 1);
  if (!ptr) {
    return 0;
  }

  http_body->data = ptr;
  memcpy(&(http_body->data[http_body->size]), contents, realsize);
  http_body->size += realsize;
  http_body->data[http_body->size] = 0;

  return realsize;
}

CURLcode
fetch(char* url, FETCH_HttpBody* http_body)
{
  CURL* curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteHttpBodyCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)http_body);

  CURLcode res = curl_easy_perform(curl_handle);

  curl_easy_cleanup(curl_handle);

  return res;
}
