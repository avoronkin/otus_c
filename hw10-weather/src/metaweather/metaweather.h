#ifndef METAWEATHER_API_H_INCLUDED
#define METAWEATHER_API_H_INCLUDED

#include <stdlib.h>

typedef enum
{
  MW_OK = 0,
  MW_NETWORK_ERROR,
  MW_JSON_ERROR,
  MW_ALLOCATION_ERROR,
} MW_Code;

typedef struct MW_Location
{
  char* title;
  char* location_type;
  int woeid;
  char* latt_long;
} MW_Location;

typedef struct MW_Locations
{
  MW_Location** entries;
  size_t size;
} MW_Locations;

typedef struct MW_Forecast
{
  int id;
  char* weather_state_name;
  char* weather_state_abbr;
  char* wind_direction_compass;
  char* created;
  char* applicable_date;
  float min_temp;
  float max_temp;
  float wind_speed;
  int wind_direction;
  int air_pressure;
  int humidity;
  float visibility;
  int predictability;
} MW_Forecast;

typedef struct MW_Forecasts
{
  MW_Forecast** entries;
  size_t size;
} MW_Forecasts;

MW_Locations*
mw_locations_alloc();

void
mw_location_free(MW_Location* location);

void
mw_locations_free(MW_Locations* locations);

MW_Code
mw_locations_search(MW_Locations* locations, char* location_name);

MW_Code
mw_locations_fill_from_response(MW_Locations* locations, char* body);

MW_Forecasts*
mw_forecasts_alloc();

void
mw_forecast_free(MW_Forecast* forecast);

void
mw_forecasts_free(MW_Forecasts* forecasts);

MW_Code
mw_forecasts_fetch(MW_Forecasts* forecasts, int woeid);

MW_Code
mw_forecasts_fill_from_response(MW_Forecasts* forecasts, char* body);

#endif // METAWEATHER_API_H_INCLUDED
