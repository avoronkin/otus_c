#include "metaweather.h"
#include "http.h"
#include "parson.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

MW_Locations*
mw_locations_alloc()
{
  MW_Locations* locations = calloc(1, sizeof(MW_Locations));
  if (locations == NULL) {
    return NULL;
  }

  return locations;
}

MW_Code
mw_locations_search(MW_Locations* locations, char* location_name)
{
  Http_ResBody http_res_body;
  http_res_body.data = malloc(1);
  http_res_body.size = 0;

  char* endpoint_url = "https://www.metaweather.com/api/location/search";

  int url_length = snprintf(NULL, 0, "%s/?query=%s", endpoint_url, location_name);
  char* url = malloc(url_length + 1);
  snprintf(url, url_length + 1, "%s/?query=%s", endpoint_url, location_name);

  CURLcode curl_code = http_get(url, &http_res_body);
  free(url);
  if (curl_code != CURLE_OK) {
    free(http_res_body.data);
    return MW_NETWORK_ERROR;
  }

  MW_Code mw_code = mw_locations_fill_from_response(locations, http_res_body.data);
  if (mw_code != MW_OK) {
    free(http_res_body.data);
    return mw_code;
  }

  free(http_res_body.data);
  return MW_OK;
}

MW_Code
mw_locations_fill_from_response(MW_Locations* locations, char* body)
{
  JSON_Value* json_value = json_parse_string(body);
  if (json_value_get_type(json_value) != JSONArray) {
    json_value_free(json_value);
    return MW_JSON_ERROR;
  }

  JSON_Array* locations_json = json_value_get_array(json_value);
  if (locations_json == NULL) {
    json_value_free(json_value);
    return MW_JSON_ERROR;
  }

  locations->size = json_array_get_count(locations_json);
  locations->entries = calloc(locations->size, sizeof(MW_Location*));

  for (size_t i = 0; i < json_array_get_count(locations_json); i++) {
    JSON_Object* location = json_array_get_object(locations_json, i);
    if (location == NULL) {
      json_value_free(json_value);
      return MW_JSON_ERROR;
    }

    MW_Location* new_location = (MW_Location*)malloc(sizeof(MW_Location));
    if (new_location == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }

    const char* title = json_object_get_string(location, "title");
    new_location->title = (char*)malloc(strlen(title) + 1);
    if (new_location->title == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }
    strcpy(new_location->title, title);

    const char* location_type = json_object_get_string(location, "location_type");
    new_location->location_type = (char*)malloc(strlen(location_type) * sizeof(char) + 1);
    if (new_location->location_type == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }
    strcpy(new_location->location_type, location_type);

    new_location->woeid = json_object_get_number(location, "woeid");

    const char* latt_long = json_object_get_string(location, "latt_long");
    new_location->latt_long = (char*)malloc(strlen(latt_long) * sizeof(char) + 1);
    if (new_location->latt_long == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }
    strcpy(new_location->latt_long, latt_long);

    locations->entries[i] = new_location;
  }

  json_value_free(json_value);
  return MW_OK;
}

void
mw_location_free(MW_Location* location)
{
  free(location->title);
  free(location->location_type);
  free(location->latt_long);
  free(location);
}

void
mw_locations_free(MW_Locations* locations)
{
  if (locations == NULL) {
    return;
  }

  for (size_t i = 0; i < locations->size; i++) {
    mw_location_free(locations->entries[i]);
  }

  free(locations->entries);
  free(locations);
}
