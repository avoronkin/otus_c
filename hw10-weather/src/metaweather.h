#ifndef METAWEATHER_API_H_INCLUDED
#define METAWEATHER_API_H_INCLUDED 1

#include <stdlib.h>

typedef enum
{
  MW_OK = 0,
  MW_NETWORK_ERROR,
  MW_JSON_ERROR,
  MW_ALLOCATION_ERROR,
} MW_code;

typedef struct MW_location
{
  char* title;
  char* location_type;
  int woeid;
  char* latt_long;
} MW_location;

typedef struct MW_locations
{
  MW_location** entries;
  size_t size;
} MW_locations;

MW_code
mw_locations_alloc(MW_locations** locations);

void
mw_location_free(MW_location* location);

void
mw_locations_free(MW_locations* locations);

MW_code
mw_locations_search(MW_locations* locations, char* location_name);

MW_code
mw_locations_fill_from_response(MW_locations* locations, char* body);

typedef struct MW_forecast
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
} MW_forecast;

typedef struct MW_forecasts
{
  MW_forecast** entries;
  size_t size;
} MW_forecasts;

MW_code
mw_forecasts_alloc(MW_forecasts** forecasts);

void
mw_forecast_free(MW_forecast* forecast);

void
mw_forecasts_free(MW_forecasts* forecasts);

MW_code
mw_forecasts_fetch(MW_forecasts* forecasts, int woeid);

MW_code
mw_forecasts_fill_from_response(MW_forecasts* forecasts, char* body);

#endif // METAWEATHER_API_H_INCLUDED
