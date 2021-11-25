#include "http.h"
#include "metaweather.h"
#include "parson.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

MW_Forecasts*
mw_forecasts_alloc()
{
  MW_Forecasts* forecasts = malloc(sizeof(MW_Forecasts));
  if (forecasts == NULL) {
    return NULL;
  }

  forecasts->size = 0;
  forecasts->entries = NULL;

  return forecasts;
}

MW_Code
mw_forecasts_fetch(MW_Forecasts* forecasts, int woeid)
{
  Http_ResBody http_res_body;
  http_res_body.data = malloc(1);
  http_res_body.size = 0;

  char* endpoint_url = "https://www.metaweather.com/api/location";
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  int url_length =
    snprintf(NULL, 0, "%s/%d/%d/%d/%d/", endpoint_url, woeid, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  char* url = malloc(url_length + 1);
  snprintf(url, url_length + 1, "%s/%d/%d/%d/%d/", endpoint_url, woeid, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  CURLcode curl_code = http_get(url, &http_res_body);
  free(url);
  if (curl_code != CURLE_OK) {
    free(http_res_body.data);
    return MW_NETWORK_ERROR;
  }

  MW_Code mw_code = mw_forecasts_fill_from_response(forecasts, http_res_body.data);
  if (mw_code) {
    free(http_res_body.data);
    return mw_code;
  }

  free(http_res_body.data);
  return MW_OK;
}

MW_Code
mw_forecasts_fill_from_response(MW_Forecasts* forecasts, char* body)
{
  JSON_Value* json_value = json_parse_string(body);
  if (json_value_get_type(json_value) != JSONArray) {
    json_value_free(json_value);
    return MW_JSON_ERROR;
  }

  JSON_Array* forecasts_json = json_value_get_array(json_value);
  if (forecasts_json == NULL) {
    json_value_free(json_value);
    return MW_JSON_ERROR;
  }

  forecasts->size = json_array_get_count(forecasts_json);
  forecasts->entries = calloc(forecasts->size, sizeof(MW_Forecast*));

  for (size_t i = 0; i < json_array_get_count(forecasts_json); i++) {
    JSON_Object* forecast_obj = json_array_get_object(forecasts_json, i);
    if (forecast_obj == NULL) {
      json_value_free(json_value);
      return MW_JSON_ERROR;
    }

    MW_Forecast* forecast = (MW_Forecast*)malloc(sizeof(MW_Forecast));
    if (forecast == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }

    const char* weather_state_name = json_object_get_string(forecast_obj, "weather_state_name");
    forecast->weather_state_name = (char*)malloc(strlen(weather_state_name) * sizeof(char) + 1);
    if (forecast->weather_state_name == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }
    strcpy(forecast->weather_state_name, weather_state_name);

    const char* wind_direction_compass = json_object_get_string(forecast_obj, "wind_direction_compass");
    forecast->wind_direction_compass = (char*)malloc(strlen(wind_direction_compass) * sizeof(char) + 1);
    if (forecast->wind_direction_compass == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }
    strcpy(forecast->wind_direction_compass, wind_direction_compass);

    const char* applicable_date = json_object_get_string(forecast_obj, "applicable_date");
    forecast->applicable_date = (char*)malloc(strlen(applicable_date) * sizeof(char) + 1);
    if (forecast->applicable_date == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }
    strcpy(forecast->applicable_date, applicable_date);

    const char* created = json_object_get_string(forecast_obj, "created");
    forecast->created = (char*)malloc(strlen(created) * sizeof(char) + 1);
    if (forecast->created == NULL) {
      json_value_free(json_value);
      return MW_ALLOCATION_ERROR;
    }
    strcpy(forecast->created, created);

    forecast->wind_direction = json_object_get_number(forecast_obj, "wind_direction");
    forecast->wind_speed = json_object_get_number(forecast_obj, "wind_speed");
    forecast->air_pressure = json_object_get_number(forecast_obj, "air_pressure");
    forecast->min_temp = json_object_get_number(forecast_obj, "min_temp");
    forecast->max_temp = json_object_get_number(forecast_obj, "max_temp");

    forecasts->entries[i] = forecast;
  }

  json_value_free(json_value);
  return MW_OK;
}

void
mw_forecast_free(MW_Forecast* forecast)
{
  if (forecast == NULL) {
    return;
  }

  free(forecast->weather_state_name);
  free(forecast->wind_direction_compass);
  free(forecast->applicable_date);
  free(forecast->created);

  free(forecast);
}

void
mw_forecasts_free(MW_Forecasts* forecasts)
{
  if (forecasts == NULL) {
    return;
  }

  for (size_t i = 0; i < forecasts->size; i++) {
    mw_forecast_free(forecasts->entries[i]);
  }

  free(forecasts->entries);
  free(forecasts);
}
