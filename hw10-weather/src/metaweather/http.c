#include "http.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

static size_t
WriteHttpResBodyCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
  size_t realsize = size * nmemb;
  Http_ResBody* http_res_body = (Http_ResBody*)userp;

  char* ptr = realloc(http_res_body->data, http_res_body->size + realsize + 1);
  if (!ptr) {
    return 0;
  }

  http_res_body->data = ptr;
  memcpy(&(http_res_body->data[http_res_body->size]), contents, realsize);
  http_res_body->size += realsize;
  http_res_body->data[http_res_body->size] = 0;

  return realsize;
}

CURLcode
http_get(char* url, Http_ResBody* http_res_body)
{
  CURL* curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteHttpResBodyCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)http_res_body);

  CURLcode res = curl_easy_perform(curl_handle);

  curl_easy_cleanup(curl_handle);

  return res;
}
